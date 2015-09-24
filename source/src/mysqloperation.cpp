#include"mysqloperation.h"
#include<iostream>
#include <mysql.h>
#include <cstring>
using namespace std;
using namespace edwin;
MysqlOperation::MysqlOperation()
{
	connection = NULL;
	res = NULL;
	result = NULL;
	line_num = 0;
}
MysqlOperation::~MysqlOperation()
{
		close();
}

//连接数据库
int MysqlOperation::connect(const char *account, const char *password, const char *database)
{
		int ret=0;
		connection = mysql_init(&mysql);
		if(connection ==NULL)
		{
			ret=-1;
			printf("connect error!\n");
			return ret;
	
		}
		connection=mysql_real_connect(connection,"localhost",account,password,database,0,NULL,0);
		if(connection==NULL)
		{
			cout<<mysql_error(&mysql)<<endl;
			cout<<"connect error"<<endl;
			return -2;
		}
		mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
		printf("connect!\n");
		return ret;

}
int MysqlOperation::create_table(const char *query)
{
	int ret=mysql_query(&mysql,query);
	if(ret!=0)
	{
		cout<<"create table error"<<endl;
		return -1;

	}
	return 0;
}
int MysqlOperation::drop_table(const char *query)
{
	int ret=mysql_query(&mysql,query);
	if(ret!=0)
	{
		cout<<"drop table error"<<endl;
		return -1;
	}
	return 0;
}
//查询数据库
int MysqlOperation::select(const char* query)
{
	int ret;
	ret=mysql_query(&mysql,query);
	if(ret!=0)
	{
		cout<<"init error"<<endl;
		return -1;

	}
	//if(mysql_next_result(&mysql)>0)
	//{
	//	cout<<"nothing"<<endl;
	//	return 1;

	//}
	result=mysql_store_result(&mysql);
	
	line_num=mysql_num_rows(result); 
	MYSQL_ROW row=NULL;
	
	int i=0;
	res=new char**[line_num];
	while(row=mysql_fetch_row(result))
	{
		res[i]=row;
		i++;
	}

	return 0;

}
//插入数据库
int MysqlOperation::insert(const char* query)
{
	cout<<"insert\n";
	int ret=mysql_query(&mysql,query);
	if(ret!=0)
	{
		cout<<"mysql insert error"<<endl;
		return -1;

	}
	return 0;
}

//修改数据库数据
int MysqlOperation::update(const char* query)
{
	int ret=mysql_real_query(&mysql,query, strlen(query));
	if(ret!=0)
	{
		cout<<"update error"<<endl;
		return -1;

	}
	return 0;
}
//删除数据库数据
int MysqlOperation::drop(const char* query)
{
	int ret=mysql_real_query(&mysql,query, strlen(query));
	if(ret!=0)
	{
		cout<<"drop error"<<endl;
		return -1;

	}

	return 0;
}

//释放数据库数据
void MysqlOperation::free_result()
{
	//
	mysql_free_result(result);
	
	//�ͷ�����ָ��
	delete []res;
}
//关闭数据库
void MysqlOperation::close()
{
	free_result();
	mysql_close(&mysql);

}
//判断是否连接数据库
bool MysqlOperation::is_closed()
{
	return mysql_ping(&mysql);
}


