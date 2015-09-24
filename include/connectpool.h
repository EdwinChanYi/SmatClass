/*
 * connectpool.h
 *
 *  Created on: Jul 28, 2015
 *      Author: miller
 */

#ifndef MYCODE_INCLUDE_CONNECTPOOL_H_
#define MYCODE_INCLUDE_CONNECTPOOL_H_
#include <string>
#include <mutex>
#include <list>
#include "mysqloperation.h"
using namespace std;
using edwin::MysqlOperation;
namespace edwin
{
	class ConnectionPool
	{
	private:
		//当前连接池大小
		int curSize;
		//连接池最大数
		int maxSize;

		string	username;
		string	password;
		string  database;
		//连接池链表
		list<MysqlOperation*>connection_list;

		mutex mtx;
	public:

		ConnectionPool(string cusername,string cpassword,string cdatabase,int cmaxSize,int iInitialSize);
		~ConnectionPool();

		//创建连接
		MysqlOperation* create_connection();
		//初始化连接池
		void init_connection(int iInitialSize);

		//提取连接
		MysqlOperation* get_connection();

		//回收连接
		int release_connection(MysqlOperation* conn);
		//释放连接池
		void destory_connPool();
		//释放连接
		void destory_connection(MysqlOperation* conn);

	};
}

#endif /* MYCODE_INCLUDE_CONNECTPOOL_H_ */
