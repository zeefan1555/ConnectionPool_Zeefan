#pragma once
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>


using namespace std;


#include "Connection.h"



//实现连接池功能

class ConnectionPool
{

public:
	static ConnectionPool* getConnectionPool();

	shared_ptr<Connection> getConnection();

private:
	ConnectionPool();
	bool loadConfigFile();


	//运行在独立的线程, 专们负责生产连接
	void produceConnectionTask();

	//超过最大空闲时间的连接, 进行回收
	void scannerConnectionTask();

private:
	string _ip;
	unsigned short _port;
	string _username;
	string _password;
	string _dbname;

private:
	int _initSize;
	int _maxSize;
	int _maxIdleTime; // 连接池的最大空闲时间
	int _connectionTimeout; // 连接获取连接的超时时间


private:
	queue<Connection*> _connectionQue;
	mutex _queueMutex;
	atomic_int _connectionCnt;  // 已连接的总数量
	condition_variable cv;  // 通知的条件变量










};