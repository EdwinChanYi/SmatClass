#ifndef TCPTEST_H
#define TCPTEST_H

#include<iostream>
#include <string>
#define SOCKET int

using std::string;
namespace test
{
	class Tcptest
	{
		public:
			Tcptest(string LocalIP, int LocalPort):m_LocalIP(LocalIP),m_LocalPort(LocalPort){}
			~Tcptest();
			bool Socket();
			bool TcpConnect(string SerIP, int SerPort);
			bool SendData(string data);
			string RecieveData();
			void CloseSocket();
			string m_LocalIP;
			int m_LocalPort;
			string m_SerIP;
			int m_SerPort;
		private:
			SOCKET socketfd;
			
	};

}
#endif
