#include <iostream>
#include <string>
#include "tsocket.h"

int main(void)
{
	using namespace std;
	using namespace Net;
	string IP("192.168.1.249");
	Tsocket tcp(IP, 8000, 1);
	if(!tcp.Socket())
	{
		cout<<"socket fail\n";
		return -1;
	}
	
	if(tcp.BindSocket())
		return -1;
	tcp.ServerConfigure();
	string data("I love you!!!");
	tcp.SendData(data);
	string temp_str;
	string recieve_str;
	while(!(temp_str = tcp.RecieveData()).empty())
	{
		cout<<temp_str;
		recieve_str += temp_str;
		cout<<recieve_str;
		break;
	}
	cout<<recieve_str;
	return 0;
}

























