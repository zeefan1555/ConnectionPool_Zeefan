#include "pch.h"
#include "Connection.h"
#include <iostream>
#include <fstream>


Connection::Connection()
{
	//�������ݿ�
	_conn = mysql_init(nullptr);
}


Connection:: ~Connection()
{
	//�ͷ����ݿ��������Դ
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port,
	string username, string password, string dbname)
{

	// �������ݿ�
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr;



}



bool Connection::update(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		std::cout << "����ʧ��:" << sql.c_str() << endl;

		return false;

	}
}


MYSQL_RES* Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		std::cout << "��ѯʧ�ܣ�" << sql.c_str() << endl;
		return nullptr;
	}
	return mysql_use_result(_conn);

}
