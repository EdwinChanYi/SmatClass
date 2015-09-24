#include <iostream>
#include <string>
#include "tcptest.h"

int main(void)
{
	using namespace std;
	using namespace test;
	string IP("192.168.209.131");
	Tcptest tcp(IP, 8000);
	if(!tcp.Socket())
	{
		cout<<"socket fail\n";
		return -1;
	}
	string serIP("192.168.1.249");
	tcp.TcpConnect(serIP, 8000);
	string temp_str;
	string recieve_str;
	while(!(temp_str = tcp.RecieveData()).empty())
	{
		cout<<temp_str;
		recieve_str =recieve_str + temp_str;
		break;
	}
	cout<<recieve_str;
	string data("I love you too!!!");
	tcp.SendData(data);
	tcp.CloseSocket();
	return 0;
}
