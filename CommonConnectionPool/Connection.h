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

	//���� insert, delete, update
	bool update(string  sql);

	//�� select
	MYSQL_RES* query(string sql);

	void refreshAliveTime() { _alivetime = clock(); }
	clock_t getAliveeTime() const { return clock() - _alivetime; }

private:
	MYSQL* _conn; // ��ʾmysql ��һ������
	clock_t _alivetime;  // ��¼����״̬��Ĵ��ʱ��




};


