/*
 * communication.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: miller
 */
#include "communication.h"
#include "tsocket.h"
#include "main.h"
#include "handle.h"
#include "homework.h"
#include <memory>
#include <mutex>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <condition_variable>
#include <iterator>
using namespace std;
using namespace edwin;

void Communication::recieve_data(Tsocket tsocket)
{
	cout<<"recieve data\n";
	UserSockInfo get_sock_info;
	UserInfo recieve_info;
	while(1)
	{
		cout<<"communication data\n";
//		accept_all_sockfd_mutex.lock();

			unique_lock<mutex> all_sockfd_lck(accept_all_sockfd_mutex);

				cout<<"wait accept socket\n";
				accept_all_sockfd_cond.wait( all_sockfd_lck,[&]{
					return !accept_all_sockfd.empty();
				});
				cout<<"wait accept socket ok\n";
				get_sock_info= accept_all_sockfd.front();
				accept_all_sockfd.pop_front();
				all_sockfd_lck.unlock();

//		accept_all_sockfd_mutex.unlock();
		string recieve = file_socket.recieve_data(get_sock_info.sockfd);
		cout<<"recieve accept socket\n";
		recieve_info.info = recieve;
		recieve_info.remoteIP = get_sock_info.remoteIP;
		recieve_info.remote_port = get_sock_info.remote_port;
		recieve_info.sockfd = get_sock_info.sockfd;
		cout<<get_sock_info.sockfd<<endl;
//		close(get_sock_info.sockfd);
//		all_user_info_mutex.lock();

			cout<<"recieve data push\n";
			unique_lock<mutex>  all_info_lck(all_user_info_mutex);
			all_user_info.push_back(recieve_info);

			all_user_info_cond.notify_one();
			all_info_lck.unlock();

//		all_user_info_mutex.unlock();
	}
}
void Communication::dispose_data(Tsocket tsocket)
{
	UserInfo dispose_info;      //用于存放用户信息
	UserInfo return_info;        //用于存放用于返回的用户信息
	string pro;                             //存放协议
	Protocol get_protocol;     //解析后的协议
	list<string> content;         //用户发来的内容
	/*作业管理所有类*/
	SubmitHomework basic_homework;
	PublishHomework send_homework;
	ViewTeacherHomework view_teacher_homework;
	ViewStudentHomework view_student_homework;
	DoHomework do_homework;
	CheckHomework check_homework;
	ViewHomeworkSituation view_homework_situation;
	string return_data;//返回数据
	string second_content; //用户内容第二次解析数据
	cout<<"dispose data\n";
	/*在接受到的用户信息队列，拿一个数据*/
//	all_user_info_mutex.lock();
	while(1)
	{
		unique_lock<mutex>  all_info_lck(all_user_info_mutex);
		cout<<"all user info cond lock\n";
		all_user_info_cond.wait(all_info_lck,[&]{
			return !all_user_info.empty();
		});
		cout<<"all user info cond wait ok\n";
		cout<<"all user info take\n";
		dispose_info = all_user_info.front();
		all_user_info.pop_front();
		all_info_lck.unlock();


//	all_user_info_mutex.unlock();

	list<string>::iterator it ;
	/*解析用户发来信息*/
	cout<<dispose_info.info<<endl;
	unsigned int match_pos = dispose_info.info.find('#');
	pro.append(dispose_info.info,0,match_pos+1);
	get_protocol.content.append(dispose_info.info.begin()+match_pos+1,dispose_info.info.end());
	int pro_num = atoi(pro.c_str());
	cout<<pro_num<<endl;
	/*分离协议*/
	get_protocol.message_type = pro_num&(0xf);
	get_protocol.subsystem = pro_num&(0xff);
	get_protocol.module = pro_num&(0x1fff);
	get_protocol.module_operate = pro_num;
	cout<<"dispose now\n";
	if(get_protocol.message_type ==0)
	{
		cout<<"message type\n";
		dispose_info.info = "message";
		tsocket.send_data(dispose_info.info,dispose_info.sockfd);
		close(dispose_info.sockfd);
	}
	else if(get_protocol.message_type == 1)
	{
		switch(get_protocol.subsystem)
		{
		case HOMEWORK:
			switch(get_protocol.module)
			{
			/*作业提交模块*/
				case BASIC_HOMEWORK:
					/*保存作业*/
					cout<<"save homework\n";
					if(get_protocol.module_operate == BASIC_HOMEWORK_SUBMIT)
					{
						cout<<"basic homework submit\n";
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,'@');
						it = content.begin();
						basic_homework.homeworkname = *(it++);
						basic_homework.teacherid = atoi((*(it++)).c_str());
						basic_homework.teacherpath = *(it++);
						basic_homework.homeworkcontent = *(it++);
						basic_homework.title_num = atoi((*(it++)).c_str());
						basic_homework.answer = *(it);
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(basic_homework.save_homework())
						{
								return_info.info = Handle::int_to_string(BASIC_HOMEWORK_SUBMIT)+"#"+Handle::int_to_string(1);
						}
						else
								return_info.info = Handle::int_to_string(BASIC_HOMEWORK_SUBMIT)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						cout<<"close socket\n";
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();
						*/
					}
					/*修改作业*/
					else if(get_protocol.module_operate == BASIC_HOMEWORK_AMEND)
					{
					    /*获取修改码*/
						cout<<"basic amend homework\n";
						match_pos = get_protocol.content.find('#');
						pro.append(get_protocol.content,0,match_pos+1);
						pro_num = atoi(pro.c_str());
						second_content.append(get_protocol.content.begin()+match_pos+1,get_protocol.content.end());
						/*初始化类*/
						content = edwin::Handle::c_to_list(second_content,'@');
						it = content.begin();
						basic_homework.homeworkid = atoi( (*(it++)).c_str() );
						basic_homework.homeworkname = *(it++);
						basic_homework.teacherpath =  *(it++);
						basic_homework.homeworkcontent =*(it++);
						basic_homework.answer = *it;
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(basic_homework.amend_homework(pro_num))
						{
							return_info.info = Handle::int_to_string(BASIC_HOMEWORK_AMEND)+"#"+Handle::int_to_string(1);
						}
						else
							return_info.info = Handle::int_to_string(BASIC_HOMEWORK_AMEND)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						cout<<"close socket\n";
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/

					}
					/*删除作业*/
					else if(get_protocol.module_operate == BASIC_HOMEWORK_DROP)
					{
						/*初始化类*/
						basic_homework.homeworkid = atoi(get_protocol.content.c_str());
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(basic_homework.delete_homework())
						{

							return_info.info = Handle::int_to_string(BASIC_HOMEWORK_DROP)+"#"+Handle::int_to_string(1);
						}
						else
							return_info.info = Handle::int_to_string(BASIC_HOMEWORK_DROP)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/
					}
					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}

						break;
						/*作业发布模块*/
				case SEND_HOMEWORK:
					/*发布作业*/
					if(get_protocol.module_operate == SEND_HOMEWORK_SEND)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						send_homework.homeworkid =atoi((*(it++)).c_str());
						send_homework.classid = atoi((*(it++)).c_str());
						send_homework.deadline =*(it++);
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(send_homework.publish_homework())
						{

							return_info.info = Handle::int_to_string(SEND_HOMEWORK_SEND)+"#"+Handle::int_to_string(1);
						}
						else
							return_info.info = Handle::int_to_string(SEND_HOMEWORK_SEND)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/
					}
					/*修改发布*/
					else if(get_protocol.module_operate == SEND_HOMEWORK_AMEND)
					{
						/*初始化类*/
						match_pos = get_protocol.content.find('#');
						pro.append(get_protocol.content,0,match_pos+1);
						pro_num = atoi(pro.c_str());
						second_content.append(get_protocol.content.begin()+match_pos,get_protocol.content.end());
						content = edwin::Handle::c_to_list(second_content,':');
						it = content.begin();
						send_homework.homeworkid =atoi((*(it++)).c_str());
						send_homework.classid = atoi((*(it++)).c_str());
						send_homework.homeworkname = *(it++);
						send_homework.new_classid = atoi((*(it++)).c_str());
						send_homework.deadline = *(it++);
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(send_homework.amend_homework(pro_num))
						{
							return_info.info = Handle::int_to_string(SEND_HOMEWORK_AMEND)+"#"+Handle::int_to_string(1);
						}
						else

							return_info.info = Handle::int_to_string(SEND_HOMEWORK_AMEND)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/

					}
					/*删除发布*/
					else if(get_protocol.module_operate ==SEND_HOMEWORK_DELETE)
					{

						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						send_homework.homeworkid =atoi((*(it++)).c_str());
						send_homework.classid = atoi((*(it++)).c_str());
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(send_homework.delete_homework())
						{
							return_info.info = Handle::int_to_string(SEND_HOMEWORK_DELETE)+"#"+Handle::int_to_string(1);
							//send_tcp_mutex.lock();

							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(SEND_HOMEWORK_DELETE)+"#"+Handle::int_to_string(0);
						}
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/

					}
					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}
						break;
				case TEACHER_VIEW:
					if(get_protocol.module_operate == TEACHER_VIEW_SUBMIT_HOMEWORK)
					{
						/*初始化类*/
						view_teacher_homework.teacherid =atoi(get_protocol.content.c_str());
						return_data = view_teacher_homework.view_submit();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
								return_info.info =Handle::int_to_string(TEACHER_VIEW_SUBMIT_HOMEWORK) +"#"+return_data;
								//send_tcp_mutex.lock();
								unique_lock<mutex>  tcp_lock(send_tcp_mutex);
								send_tcp_info.push_back(return_info);
								send_tcp_cond.notify_one();
								 tcp_lock.unlock();
								//send_tcp_mutex.unlock();
						}
						else
						{
								return_info.info = Handle::int_to_string(TEACHER_VIEW_SUBMIT_HOMEWORK) +"#"+return_data;
								tsocket.send_data(return_info.info,dispose_info.sockfd);
								close(dispose_info.sockfd);
								/*
								send_udp_mutex.lock();
								send_udp_info.push_back(return_info);
								send_udp_cond.notify_one();
								send_udp_mutex.unlock();*/
						}

					}
					else if(get_protocol.module_operate == TEACHER_VIEW_SEND_HOMEWORK)
					{
						/*初始化类*/
						view_teacher_homework.teacherid =atoi(get_protocol.content.c_str());
						return_data = view_teacher_homework.view_publish();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(TEACHER_VIEW_SEND_HOMEWORK) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(TEACHER_VIEW_SEND_HOMEWORK) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}
					else if(get_protocol.module_operate == TEACHER_VIEW_HOMEWORK_CONTENT)
					{
						/*初始化类*/
						view_teacher_homework.homeworkid =atoi(get_protocol.content.c_str());
						return_data = view_teacher_homework.view_homework_content();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(TEACHER_VIEW_HOMEWORK_CONTENT) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(TEACHER_VIEW_HOMEWORK_CONTENT) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}
					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}
					break;
				case CHECK_HOMEWORK:
					if(get_protocol.module_operate == CHECK_HOMEWORK_SUBJECT)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						check_homework.homeworkid =atoi((*(it++)).c_str());
						check_homework.classid = atoi((*(it++)).c_str());
						return_data = check_homework.get_subjet_answer();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(CHECK_HOMEWORK_SUBJECT) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(CHECK_HOMEWORK_SUBJECT) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}
					else if(get_protocol.module_operate == CHECK_HOMEWORK_SET_SUBJECT_SCORE)
					{
						content = edwin::Handle::c_to_list(get_protocol.content,'@');
						it = content.begin();
						match_pos = (*it).find(',');
						check_homework.homeworkid = atoi(pro.append((*it).begin(),(*it).begin()+match_pos).c_str());
						check_homework.scale = atoi(pro.append((*it).begin()+match_pos+2,(*it).end()).c_str());
						it++;
						check_homework.student_score = (*it);
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						if(check_homework.set_subject_score())
						{
							return_info.info = Handle::int_to_string(CHECK_HOMEWORK_SET_SUBJECT_SCORE)+"#"+Handle::int_to_string(1);
						}
						else
							return_info.info = Handle::int_to_string(CHECK_HOMEWORK_SET_SUBJECT_SCORE)+"#"+Handle::int_to_string(0);
						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/
					}
					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/

					}
					break;
				case STUDENT_VIEW:
					if(get_protocol.module_operate == STUDENT_VIEW_HOMEWORK_LIST)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						view_student_homework.studentid =atoi((*(it++)).c_str());
						view_student_homework.homework_state = atoi((*(it)).c_str());
						return_data = view_student_homework.show_list();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(STUDENT_VIEW_HOMEWORK_LIST) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(STUDENT_VIEW_HOMEWORK_LIST) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}

					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}
						break;
				case STUDENT_TEST:
					if(get_protocol.module_operate == GET_HOMEWORK_CONTENT)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						do_homework.homeworkid = atoi((*(it++)).c_str());
						do_homework.studentid =atoi((*(it++)).c_str());

						return_data =do_homework.get_homework();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(GET_HOMEWORK_CONTENT) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{

							return_info.info = Handle::int_to_string(GET_HOMEWORK_CONTENT) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}
					else if(get_protocol.module_operate == HANDIN_HOMEWORK_REPORT)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,'@');
						it = content.begin();
						do_homework.studentid = atoi((*(it++)).c_str());
						do_homework.classid =atoi((*(it++)).c_str());
						do_homework.homeworkid =atoi((*(it++)).c_str());
						do_homework.choice_answer = *(it++);
						do_homework.object_answer = *it;
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(do_homework.hand_in_homework())
						{
							return_info.info = Handle::int_to_string(HANDIN_HOMEWORK_REPORT) +"#"+Handle::int_to_string(1);
						}
						else
							return_info.info = Handle::int_to_string(HANDIN_HOMEWORK_REPORT) +"#"+Handle::int_to_string(0);

						tsocket.send_data(return_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);
						/*
						send_udp_mutex.lock();
						send_udp_info.push_back(return_info);
						send_udp_cond.notify_one();
						send_udp_mutex.unlock();*/
					}

					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}
						break;
				case VIEW_HOMEWORK:
					if(get_protocol.module_operate == VIEW_HOMEWORK_CLASS_HANDIN)
					{
						/*初始化类*/
							view_homework_situation.classid =atoi(get_protocol.content.c_str());
							return_data = view_homework_situation.show_class_hand_in();
							return_info.remoteIP = dispose_info.remoteIP;
							return_info.remote_port = dispose_info.remote_port;
							return_info.sockfd = dispose_info.sockfd;
							if(!return_data.empty())
							{
								return_info.info =Handle::int_to_string(VIEW_HOMEWORK_CLASS_HANDIN) +"#"+return_data;
								//send_tcp_mutex.lock();
								unique_lock<mutex>  tcp_lock(send_tcp_mutex);
								send_tcp_info.push_back(return_info);
								send_tcp_cond.notify_one();
								 tcp_lock.unlock();
								//send_tcp_mutex.unlock();
							}
							else
							{
								return_info.info = Handle::int_to_string(VIEW_HOMEWORK_CLASS_HANDIN) +"#"+return_data;
								tsocket.send_data(return_info.info,dispose_info.sockfd);
								close(dispose_info.sockfd);
								/*
								send_udp_mutex.lock();
								send_udp_info.push_back(return_info);
								send_udp_cond.notify_one();
								send_udp_mutex.unlock();*/
							}
					}
					else if(get_protocol.module_operate == VIEW_HOMEWORK_STUDENT_HANDIN)
					{
						/*初始化类*/
							view_homework_situation.studentid =atoi(get_protocol.content.c_str());
							return_data = view_homework_situation.show_student_hand_in();
							return_info.remoteIP = dispose_info.remoteIP;
							return_info.remote_port = dispose_info.remote_port;
							return_info.sockfd = dispose_info.sockfd;
							if(!return_data.empty())
							{
								return_info.info =Handle::int_to_string(VIEW_HOMEWORK_STUDENT_HANDIN) +"#"+return_data;
								//send_tcp_mutex.lock();
								unique_lock<mutex>  tcp_lock(send_tcp_mutex);
								send_tcp_info.push_back(return_info);
								send_tcp_cond.notify_one();
								 tcp_lock.unlock();
								//send_tcp_mutex.unlock();
							}
							else
							{
								return_info.info = Handle::int_to_string(VIEW_HOMEWORK_STUDENT_HANDIN) +"#"+return_data;
								tsocket.send_data(return_info.info,dispose_info.sockfd);
								close(dispose_info.sockfd);
								/*
								send_udp_mutex.lock();
								send_udp_info.push_back(return_info);
								send_udp_cond.notify_one();
								send_udp_mutex.unlock();*/
							}
					}
					else if(get_protocol.module_operate == VIEW_HOMEWORK_CLASS_SCORE_RANK)
					{
						/*初始化类*/
						content = edwin::Handle::c_to_list(get_protocol.content,':');
						it = content.begin();
						view_homework_situation.classid = atoi((*(it++)).c_str());
						view_homework_situation.homeworkid =atoi((*(it)).c_str());

						return_data =view_homework_situation.show_class_score_rank();
						return_info.remoteIP = dispose_info.remoteIP;
						return_info.remote_port = dispose_info.remote_port;
						return_info.sockfd = dispose_info.sockfd;
						if(!return_data.empty())
						{
							return_info.info =Handle::int_to_string(VIEW_HOMEWORK_CLASS_SCORE_RANK) +"#"+return_data;
							//send_tcp_mutex.lock();
							unique_lock<mutex>  tcp_lock(send_tcp_mutex);
							send_tcp_info.push_back(return_info);
							send_tcp_cond.notify_one();
							 tcp_lock.unlock();
							//send_tcp_mutex.unlock();
						}
						else
						{
							return_info.info = Handle::int_to_string(VIEW_HOMEWORK_CLASS_SCORE_RANK) +"#"+return_data;
							tsocket.send_data(return_info.info,dispose_info.sockfd);
							close(dispose_info.sockfd);
							/*
							send_udp_mutex.lock();
							send_udp_info.push_back(return_info);
							send_udp_cond.notify_one();
							send_udp_mutex.unlock();*/
						}
					}
					else if(get_protocol.module_operate == VIEW_HOMEWORK_STUDENT_SCORE_RANK)
					{
						/*初始化类*/
							view_homework_situation.studentid =atoi(get_protocol.content.c_str());
							return_data = view_homework_situation.show_student_score_rank();
							return_info.remoteIP = dispose_info.remoteIP;
							return_info.remote_port = dispose_info.remote_port;
							return_info.sockfd = dispose_info.sockfd;
							if(!return_data.empty())
							{
								return_info.info =Handle::int_to_string( VIEW_HOMEWORK_STUDENT_SCORE_RANK) +"#"+return_data;
								//send_tcp_mutex.lock();
								unique_lock<mutex>  tcp_lock(send_tcp_mutex);
								send_tcp_info.push_back(return_info);
								send_tcp_cond.notify_one();
								 tcp_lock.unlock();
								//send_tcp_mutex.unlock();
							}
							else
							{

								return_info.info = Handle::int_to_string( VIEW_HOMEWORK_STUDENT_SCORE_RANK) +"#"+return_data;
								tsocket.send_data(return_info.info,dispose_info.sockfd);
								close(dispose_info.sockfd);
								/*
								send_udp_mutex.lock();
								send_udp_info.push_back(return_info);
								send_udp_cond.notify_one();
								send_udp_mutex.unlock();*/
							}
					}
					else if(get_protocol.module_operate == VIEW_HOMEWORK_REPORT)
					{
						/*初始化类*/
							content = edwin::Handle::c_to_list(get_protocol.content,':');
							it = content.begin();
							view_homework_situation.homeworkid = atoi((*(it++)).c_str());
							view_homework_situation.classid =atoi((*(it)).c_str());

							return_data =view_homework_situation.show_homework_report_situation();
							return_info.remoteIP = dispose_info.remoteIP;
							return_info.remote_port = dispose_info.remote_port;
							return_info.sockfd = dispose_info.sockfd;
							if(!return_data.empty())
							{
								return_info.info =Handle::int_to_string( VIEW_HOMEWORK_REPORT) +"#"+return_data;
								//send_tcp_mutex.lock();
								unique_lock<mutex>  tcp_lock(send_tcp_mutex);
								send_tcp_info.push_back(return_info);
								send_tcp_cond.notify_one();
								 tcp_lock.unlock();
								//send_tcp_mutex.unlock();
							}
							else
							{

								return_info.info = Handle::int_to_string( VIEW_HOMEWORK_REPORT) +"#"+return_data;
								tsocket.send_data(return_info.info,dispose_info.sockfd);
								close(dispose_info.sockfd);
								/*
								send_udp_mutex.lock();
								send_udp_info.push_back(return_info);
								send_udp_cond.notify_one();
								send_udp_mutex.unlock();*/
							}
					}
					else
					{
						cout<<"basic homework error\n";
						dispose_info.info = "message error";
						tsocket.send_data(dispose_info.info,dispose_info.sockfd);
						close(dispose_info.sockfd);

					}
						break;
			}
			break;
			default :
				break;
		}
	}
	else
	{
		cout<<"message type error\n";
		dispose_info.info = "message error";
		tsocket.send_data(dispose_info.info,dispose_info.sockfd);
		close(dispose_info.sockfd);

	}
	}
}

void Communication::net_TCP_operate(Tsocket ii)
{
	cout<<"net_operate\n";
	if(!file_socket.create_socket())
	{
		cout<<"create socket error\n";
		return;
	}
	if(!file_socket.bind_socket())
	{
		cout<<"bind socket\n";
		return;
	}
	if(!file_socket.server_configure(10))
	{
		cout<<"server configure error\n";
		return;
	}
	UserSockInfo accept_sock;
	while(1)
	{
		cout<<"go to accept\n";
		if((accept_sock.sockfd = file_socket.accept_socket())==-1)
		{
			cout<<"accept error\n";
			continue;
		}
		accept_sock.remoteIP = file_socket.m_RemoteIP;
		accept_sock.remote_port =  file_socket.m_RemotePort;
		cout<<"accept lock\n";
		cout<<accept_sock.sockfd<<endl;
		unique_lock<mutex> all_sockfd_lck(accept_all_sockfd_mutex);
		accept_all_sockfd.push_back(accept_sock);
		accept_all_sockfd_cond.notify_one();
		all_sockfd_lck.unlock();

		cout<<"accept a socket\n";
	}
}
void Communication::tcp_send_data(Tsocket tsocket)
{
	cout<<"send tcp data\n";
	UserInfo send_info;
	//send_tcp_mutex.lock();
	unique_lock<mutex>  tcp_lock(send_tcp_mutex);

		cout<<"send lock tcp data\n";

		send_tcp_cond.wait(tcp_lock,[&]{return !send_tcp_info.empty();});


	send_info = send_tcp_info.front();
	send_tcp_info.pop_front();
	 tcp_lock.unlock();
	//send_tcp_mutex.unlock();

	if(tsocket.send_data(send_info.info,send_info.sockfd))
	{
		cout<<"send data error\n";
		//send_tcp_mutex.lock();
		unique_lock<mutex>  tcp_lock(send_tcp_mutex);
		send_tcp_info.push_back(send_info);
		send_tcp_cond.notify_one();
		tcp_lock.unlock();
		//send_tcp_mutex.unlock();
	}
	else
		close(send_info.sockfd);
}
void Communication::udp_send_data(int data)
{
	UserInfo send_info;
	send_udp_mutex.lock();
	while(send_udp_info.size() == 0)
	{
		unique_lock<mutex>  udp_lock(send_udp_mutex);
		send_udp_cond.wait(udp_lock);
	}
	send_info = send_udp_info.front();
	send_udp_info.pop_front();
	send_udp_mutex.unlock();


}
