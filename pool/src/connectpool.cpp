#include "connectpool.h"
#include<list>
#include "mysqloperation.h"
#include <iostream>
using namespace edwin;
using namespace std;
ConnectionPool::ConnectionPool(string cusername,string cpassword,string cdatabase,int cmaxSize,int iInitialSize)
{
	username=cusername;
	password=cpassword;
	database=cdatabase;
	maxSize=cmaxSize;
	curSize = 0;
	this->init_connection(iInitialSize);
}

ConnectionPool::~ConnectionPool()
{
	this->destory_connPool();
}


MysqlOperation* ConnectionPool::create_connection()
{
	MysqlOperation* mysql=new MysqlOperation();
	if(mysql->connect(this->username.c_str(), this->password.c_str(), this->database.c_str()))
	{
		cout<<"create connection\n";
		return NULL;
	}
	return mysql;
}


void ConnectionPool::init_connection(int iInitialSize)
{
	MysqlOperation* connect;
	mtx.lock();
	int i;
	for(i=0;i<iInitialSize;i++)
	{
		connect=this->create_connection();
		if(connect)
		{
			connection_list.push_back(connect);
			++(this->curSize);
		}
		else
		{
			cout<<"connection create error"<<endl;
		}
	}

	mtx.unlock();

}


//����NULL����ʧ��
MysqlOperation* ConnectionPool::get_connection()
{
	MysqlOperation* connect=NULL;
	mtx.lock();
	//���ӳ��л�������
	if(connection_list.size()>0)
	{
		connect=connection_list.front();
		connection_list.pop_front();

		if(connect->is_closed())
		{
			delete connect;
			connect=this->create_connection();
		}
	
		if(connect==NULL)
		{
			--curSize;
			cout<<"create new connection fail"<<endl;
		}
		mtx.unlock();
		return connect;
	}
	
	else
	{	
		//δ�ﵽ���������Ŀ
		if(curSize< maxSize)
		{
			connect=this->create_connection();
			if(connect)
			{
				++curSize;
				mtx.unlock();
				return connect;
			}
			//����ʧ��
			else
			{
				cout<<"create new connection fail"<<endl;
				mtx.unlock();
				return connect;
			}
		}
		
		else
		{
			cout<<"connection is full!"<<endl;
			mtx.unlock();
			return NULL;
		}

	}
	

}





int ConnectionPool::release_connection(MysqlOperation* connection)
{
	if(connection)
	{
		mtx.lock();
		connection_list.push_back(connection);
		mtx.unlock();
		return 0;
	}
	
	else
	{
		cout<<"release fail!"<<endl;
		return 1;
	}

}

//�������ӳ�,����Ҫ���������ӳص�������  
void ConnectionPool::destory_connPool(){  
    list<MysqlOperation*>::iterator icon;  
    mtx.lock();  
    for(icon=connection_list.begin();icon!=connection_list.end();++icon)  
    {  
        this->destory_connection(*icon);//�������ӳ��е�����
    }  
    curSize=0;  
    connection_list.clear();//������ӳ��е�����  
    mtx.unlock();  
}  
//����һ������  
void ConnectionPool::destory_connection(MysqlOperation* conn)  
{  
	mtx.lock();
    	if(conn)    
         	conn->close();
     	mtx.unlock();	  
}  

