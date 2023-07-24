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



//ʵ�����ӳع���

class ConnectionPool
{

public:
	static ConnectionPool* getConnectionPool();

	shared_ptr<Connection> getConnection();

private:
	ConnectionPool();
	bool loadConfigFile();


	//�����ڶ������߳�, ר�Ǹ�����������
	void produceConnectionTask();

	//����������ʱ�������, ���л���
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
	int _maxIdleTime; // ���ӳص�������ʱ��
	int _connectionTimeout; // ���ӻ�ȡ���ӵĳ�ʱʱ��


private:
	queue<Connection*> _connectionQue;
	mutex _queueMutex;
	atomic_int _connectionCnt;  // �����ӵ�������
	condition_variable cv;  // ֪ͨ����������










};