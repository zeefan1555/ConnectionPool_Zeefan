#pragma once
#include<mysql.h>
#include<string>
#include <ctime>

#include <cmath>
using namespace std;



class Connection
{
public:
	Connection();
	~Connection();

	bool connect(string ip,
		unsigned short port,
		string user,
		string password,
		string dbname);

	//更新 insert, delete, update
	bool update(string  sql);

	//查 select
	MYSQL_RES* query(string sql);

	void refreshAliveTime() { _alivetime = clock(); }
	clock_t getAliveeTime() const { return clock() - _alivetime; }

private:
	MYSQL* _conn; // 表示mysql 的一条连接
	clock_t _alivetime;  // 记录空闲状态后的存活时间




};


