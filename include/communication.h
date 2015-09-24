/*
 * communication.h
 *
 *  Created on: Jul 30, 2015
 *      Author: miller
 */

#ifndef MYCODE_INCLUDE_COMMUNICATION_H_
#define MYCODE_INCLUDE_COMMUNICATION_H_
#include "main.h"
#include "tsocket.h"

using namespace edwin;
namespace edwin
{
	class Communication
	{
	public:
		static void net_TCP_operate(Tsocket tsocket);
		static void net_UDP_operate();
		static void recieve_data(Tsocket tsocket);
		static void dispose_data(Tsocket tsocket);
		static void tcp_send_data(Tsocket tsocket);
		static void udp_send_data(int data);
	};
}



#endif /* MYCODE_INCLUDE_COMMUNICATION_H_ */
