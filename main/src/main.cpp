#include <iostream>
#include <string>
#include "mysqloperation.h"
#include "tsocket.h"
#include "homework.h"
#include "file.h"
#include <mutex>
#include <list>
#include <condition_variable>
#include "communication.h"
#include "threadpool.h"
#include "cpp11thread.h"
using namespace std;
using namespace edwin;

list<Class> class_list;
list<Member> user_list;
list<UserSockInfo> accept_all_sockfd;
list<UserInfo> all_user_info;
list<UserInfo> send_udp_info;
list<UserInfo> send_tcp_info;
mutex user_sock_info_mutex;
mutex user_info_mutex;
mutex send_udp_info_mutex;
mutex send_tcp_info_mutex;
mutex accept_all_sockfd_mutex;
mutex all_user_info_mutex;
condition_variable accept_all_sockfd_cond;
condition_variable all_user_info_cond;
mutex send_udp_mutex;
mutex send_tcp_mutex;
condition_variable send_udp_cond;
condition_variable send_tcp_cond;
ConnectionPool homework_cpool(string("root"),string("123456"), string("IntelligenceClass"),20,20);
Tsocket file_socket(string("192.168.1.249"),8000, true);
void hello(int a)
{
	cout<<"hello\n";
	ViewTeacherHomework my_view(2,4);
	string str = my_view.view_homework_content();
	cout<<str<<endl;
	CheckHomework my_check(6,2,string("106,87:107,80:108,100:105,90"),2);
	cout<<my_check.get_subjet_answer()<<endl;
	cout<<my_check.set_subject_score()<<endl;
	DoHomework my_do(105,6,2,string("A,B,C,D,A,D,C,B,D,C"),string("fdefeeerererere"));
	cout<<my_do.get_homework()<<endl;

	ViewHomeworkSituation my_view_situation;
	my_view_situation.classid = 2;
	my_view_situation.studentid = 105;
	my_view_situation.homeworkid = 6;
	str = my_view_situation.show_class_score_rank();
	cout<<str<<endl;
	str = my_view_situation.show_student_score_rank();
	cout<<str<<endl;
}

int main(int argv, char **argc)
{
	ThreadPool<Tsocket,void> get_file_message(10);
	ThreadPool<Tsocket,void> handle_message(10);
	ThreadPool<Tsocket,void> send_message(1);


	Cpp11Thread<Tsocket,void> net_thread;
//	Cpp11Thread<int,void> test_thread;

	Task<Tsocket,void> net_task;
	net_task.func = Communication::net_TCP_operate;
	net_task.arg = file_socket;

	Task<Tsocket,void> recieve_task;
	recieve_task.func = Communication::recieve_data;
	recieve_task.arg = file_socket;

	Task<Tsocket,void> dispose_task;
	dispose_task.func = Communication::dispose_data;
	dispose_task.arg = file_socket;

	Task<Tsocket,void> send_task;
	send_task.func = Communication::tcp_send_data;
	send_task.arg = file_socket;
/*
	Task<int,void> test_task;
	test_task.func = hello;
	test_task.arg = 1;
*/
	net_thread.append(net_task);
//	test_thread.append(test_task);


for(int i = 0; i < 10; i++)
{
		get_file_message.append(recieve_task);
}

	for(int i = 0; i <10; i++)
	{
	handle_message.append(dispose_task);
	}

	for(int i = 0;i<10;i++)
	{
//		send_message.append(send_task);
	}

	send_message.wait_for_idle();
	send_message.terminate();
	send_message.join();
	net_thread.wait_for_idle();
	net_thread.terminate();
	net_thread.join();
/*	test_thread.wait_for_idle();
	test_thread.terminate();
	test_thread.join();*/
	get_file_message.wait_for_idle();
	get_file_message.terminate();
	get_file_message.join();

	handle_message.wait_for_idle();
	handle_message.terminate();
	handle_message.join();
	return 0;
}

