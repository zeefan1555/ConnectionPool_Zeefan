#include "pch.h"
#include "CommonConnectionPool.h"
#include <iostream>
#include <fstream>


ConnectionPool* ConnectionPool::getConnectionPool()
{

	static ConnectionPool pool; // object
	return &pool; //pointer
}


bool ConnectionPool::loadConfigFile()
{

	ifstream file;
	file.open("mysql.ini");

	string str;

	while (getline(file, str))
	{

		int idx = str.find("=", 0);
		if (idx == -1)
			continue;

		//password = zz183173\n
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);
		if (key == "ip")
			_ip = value;
		else if (key == "port")
			_port = atoi(value.c_str());
		else if (key == "username")
			_username = value;
		else if (key == "password")
			_password = value;
		else if (key == "dbname")
			_dbname = value;
		else if (key == "initSize")
			_initSize = atoi(value.c_str());
		else if (key == "maxSize")
			_maxSize = atoi(value.c_str());
		else if (key == "maxIdleTime")
			_maxIdleTime = atoi(value.c_str());
		else if (key == "connectionTimeOut");
		_connectionTimeout = atoi(value.c_str());

	}

	return true;

}


ConnectionPool::ConnectionPool()
{
	if (!loadConfigFile())
		return;

	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();
		_connectionQue.push(p);
		_connectionCnt++;

	}

	// 生产者
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
	produce.detach();

	//定时清理
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();


}



// 生产者
void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
			cv.wait(lock);  // 队列不空, 此时生产者等待



		//队列为空， 开始生产
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();
			_connectionQue.push(p);
			_connectionCnt++;
		}

		//通知消费者
		cv.notify_all();

	}

}

shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex>lock(_queueMutex);
	while (_connectionQue.empty())
	{
		//sleep
		if (cv_status::timeout == cv.wait_for(lock, chrono::microseconds(_connectionTimeout)))
		{
			if (_connectionQue.empty())
				std::cout << "获取超时， 连接失败" << endl;
			return  nullptr;

		}
	}

	/*
	shared_ptr智能指针析构时，会把connection资源直接delete掉，相当于
	调用connection的析构函数，connection就被close掉了。
	这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue当中
	*/

	shared_ptr<Connection> sp(_connectionQue.front(),
		[&](Connection *pcon)
	{
		// 这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全操作
		unique_lock<mutex> lock(_queueMutex);
		pcon->refreshAliveTime();
		_connectionQue.push(pcon);
		// 归还原本的conn， 不用++cnt
	});

	_connectionQue.pop();
	cv.notify_all();

	return sp;
}


void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveeTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;
			}
			else
			{
				//队头没有超过， 其他连接肯定没有
				break;
			}

		}
	}
}






