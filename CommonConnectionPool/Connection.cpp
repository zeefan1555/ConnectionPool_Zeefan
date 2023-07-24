#include "pch.h"
#include "Connection.h"
#include <iostream>
#include <fstream>


Connection::Connection()
{
	//连接数据库
	_conn = mysql_init(nullptr);
}


Connection:: ~Connection()
{
	//释放数据库的连接资源
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port,
	string username, string password, string dbname)
{

	// 连接数据库
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), username.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr;



}



bool Connection::update(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		std::cout << "更新失败:" << sql.c_str() << endl;

		return false;

	}
}


MYSQL_RES* Connection::query(string sql)
{
	if (mysql_query(_conn, sql.c_str()))
	{
		std::cout << "查询失败：" << sql.c_str() << endl;
		return nullptr;
	}
	return mysql_use_result(_conn);

}
