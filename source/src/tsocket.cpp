#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include "main.h"
#include "tsocket.h"
#include <cstring>

#include "cpp11thread.h"
using namespace edwin;
using std::cout;
using std::endl;
Tsocket::~Tsocket()
{
	close_socket();
}

bool Tsocket::create_socket()
{
	if(m_bmode)
	{
		/*创建TCP socket*/
		if((socketfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
		{
			net_state = false;
			return false;
		}
		cout<<"socket success\n";
	}
	else
	{
		/*创建UDP socket*/
		if((socketfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
		{
			net_state = false;
			return false;
		}
	}
	return true;
}
Tsocket::Tsocket(const Tsocket &tsocket)
{
	this->socketfd = tsocket.socketfd;
	this->m_bmode = tsocket.m_bmode;
	this->m_LocalIP = tsocket.m_LocalIP;
	this->m_LocalPort = tsocket.m_LocalPort;
	this->m_RemoteIP = tsocket.m_RemoteIP;
	this->m_RemotePort = tsocket.m_RemotePort;
}

bool Tsocket::bind_socket()
{
	struct sockaddr_in socketaddr;
	
	if(m_bmode)
	{
		socketaddr.sin_family = AF_INET;
	    socketaddr.sin_addr.s_addr = inet_addr(m_LocalIP.c_str());
	    socketaddr.sin_port = htons(m_LocalPort);

		if(bind(socketfd, (struct sockaddr *)&socketaddr, sizeof(struct sockaddr)))
		{
			close_socket();
			cout<<"bind:error\n";
			return false;
		}
		 cout<<"bind success\n";
	}
	else
	{
		socketaddr.sin_family = AF_INET;
	    socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    socketaddr.sin_port = 0;
		if(bind(socketfd, (struct sockaddr *)&socketaddr, sizeof(struct sockaddr)))
		{
			close_socket();
			cout<<"bind:error\n";
			return false;
		}
	}
	return true;
}


bool Tsocket::server_configure(int ListenNum )
{
	if(m_bmode)
	{

		if(listen(socketfd, ListenNum))
		{
			close_socket();
			cout<<"Fail to listen\n";
			return false;
		}
		cout<<"listen success\n";
		int sin_size = 1;
		if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &sin_size, sizeof(int)))
		{
			close_socket();
			cout<<"Fail to setsockopt\n";
			return false;
		}
		cout<<"setsockopt success\n";
	}
	else
	{
		struct sockaddr_in  socketaddr; 
		socketaddr.sin_family = AF_INET;
		socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		socketaddr.sin_port = 0;
		if(setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &socketaddr, sizeof(sockaddr)))
	      {
				close_socket();
		        cout<<"Fail to get sockopt\n";
		        return false;
		}
	}
	return true;

}

int Tsocket::accept_socket()
{
	struct sockaddr_in remote_socketaddr;
	socklen_t sock_len = sizeof(struct sockaddr);
	cout<<"accept\n";
	if((accept_sockfd = accept(socketfd, (struct sockaddr *)&remote_socketaddr, &sock_len)) == -1)
	{
		cout<<"Fail to accept\n";
		return -1;
	}
	m_RemoteIP = inet_ntoa(remote_socketaddr.sin_addr);
	m_RemotePort = ntohs(remote_socketaddr.sin_port);
	return accept_sockfd;
}
int Tsocket::send_data(string &Data,int sockfd)
{
	if(m_bmode)
	{
		if(send(sockfd, Data.c_str() , strlen(Data.c_str()), 0) == -1)
		{
			cout<<"Fail to send the data.\n";
			return -1;
		}
		cout<<"send success\n";
	}
	else
	{
		struct sockaddr_in  socketaddr; 
		socketaddr.sin_family = AF_INET;
		socketaddr.sin_addr.s_addr = inet_addr(m_RemoteIP.c_str());
		socketaddr.sin_port = htons(8000);
		if(sendto(socketfd, Data.c_str(), strlen(Data.c_str()), 0, (struct sockaddr *)&socketaddr, sizeof(struct sockaddr)) == -1)
     		{
         		cout<<"Fail to send me out\n"; 
     		} 
	}
	return 0;
}

string Tsocket::recieve_data(int sockfd)
{
	string recieve;
	char buff[1000];
	if(m_bmode)
	{
		cout<<"recieve buff\n";
		while(1)
		{
//			memcpy(buff, 0, sizeof(buff));
			memset(buff,0,sizeof(buff));
			if(recv(sockfd, buff, sizeof(buff), 0) == -1)
			{
				cout<<"Fail to recieve the data\n";
			}
			recieve =recieve+ buff;
			cout<<recieve<<endl;
			if(strlen(buff)<1000)
			{
				cout<<"finish recieve\n";
				break;
			}
		}
	}
	else
	{
		struct sockaddr_in  socketaddr; 
		socklen_t sockaddrlen = sizeof(struct sockaddr);
	//	memcpy(buff, 0, sizeof(buff));
		memset(buff,0,sizeof(buff));
		 if(recvfrom(socketfd, buff, sizeof(buff), 0, (struct sockaddr *)&socketaddr, &sockaddrlen) == -1)
	     {
			 cout<<"Fail to recieve the data\n";

	     }
		 else
		 {
			 m_RemoteIP = inet_ntoa(socketaddr.sin_addr);
			 m_RemotePort = ntohs(socketaddr.sin_port);
		 }

	}
	return recieve;
}
bool Tsocket::is_connect()
{
	return net_state == true;
}
void Tsocket::close_socket()
{
	net_state = false;
	close(socketfd);
}

