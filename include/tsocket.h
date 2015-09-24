#ifndef _TSOCKET_H
#define _TSOCKET_H
#include <iostream>
#include <string>
//#include "main.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
using std::string;
namespace edwin
{
	class Tsocket
	{
	public:
		Tsocket(string LocalIP, int LocalPort, bool mode=false):m_LocalIP(LocalIP), m_LocalPort(LocalPort), m_bmode(mode)
		{
			net_state = true;
		}
		Tsocket(){}
		virtual ~Tsocket();

		Tsocket(const Tsocket &tsocket);
		/* 创建socket*/
		bool create_socket();
		/* 绑定IP和端口*/
		bool bind_socket();
		/*连接*/
		bool connect_socket();
		/* 配置网络*/
		bool server_configure(int ListenNum = 10);
		/*TCP提取接收socket*/
		int accept_socket();
		/* 发送数据*/
		int send_data(string &Data,int sockfd);
		/*接受数据*/
		string recieve_data(int sockfd);
		/*判断网络是否连接*/
		bool is_connect();
		/*close socket*/
		void close_socket();
		/* 本地IP和端口*/
		string m_LocalIP;
		int m_LocalPort;

		/* 远程IP和端口*/
		string m_RemoteIP;
		int m_RemotePort;

	private:
		bool m_bmode;// 1为TCP，2为UDP;
		int socketfd;
		int accept_sockfd;
		bool net_state;
	};
}



#endif

