#include <iostream>
#include <string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include <string.h>
#include"tcptest.h"

using namespace test;
using std::cout;
using std::endl;
Tcptest::~Tcptest()
{
	close(socketfd);
}

bool Tcptest::TcpConnect(string SerIP, int SerPort)
{
	m_SerIP = SerIP;
	m_SerPort = SerPort;
	struct sockaddr_in sockaddr;
	sockaddr.sin_addr.s_addr = inet_addr(SerIP.c_str());
	sockaddr.sin_port = htons(SerPort);
	sockaddr.sin_family = AF_INET;
	if(connect(socketfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)))
	    {
	        cout<<"Fail to connect the sock\n";
	        return false;
	    }
	cout<<"connect success\n";
	return true;
}
bool Tcptest::Socket()
{
	 if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout<<"Fail to make the socket\n";
		return false;
    }
	 return true;
}
bool Tcptest::SendData(string data)
{
	 if(send(socketfd, data.c_str(), strlen(data.c_str()), 0) == -1)
        {
             cout<<"Fail to send the data.\n";
             return false;
        }
	 cout<<"send success\n";
	return true;
}
string Tcptest::RecieveData()
{
	char buff[100];
	memset(buff, 0, 100);
	string *data;
	if(recv(socketfd, buff, sizeof(buff), 0) == -1)
        {
            cout<<"Fail to recieve the data\n";
		return NULL;
        }
	data = new string(buff);
	return *data;
}
void Tcptest::CloseSocket()
{
	close(socketfd);
}

