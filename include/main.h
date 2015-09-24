#ifndef MAIN_H
#define MAIN_H

#include "homework.h"
#include <functional>
#include <list>
#include <mutex>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <condition_variable>
#include "connectpool.h"
#include "tsocket.h"
#define HOMEPATH "/home/miller/smartclass/"

//作业管理子系统协议
/*作业子系统*/
#define HOMEWORK 0x00000041UL
/*模块*/
#define BASIC_HOMEWORK 0x00000141UL
#define SEND_HOMEWORK 0x00000241UL
#define TEACHER_VIEW 0x00000341UL
#define CHECK_HOMEWORK 0x00000441UL
#define STUDENT_VIEW 0x00000541UL
#define STUDENT_TEST 0x00000641UL
#define VIEW_HOMEWORK 0x00000741UL

/*作业提交*/
#define BASIC_HOMEWORK_SUBMIT 0x00002141UL
#define BASIC_HOMEWORK_AMEND 0x00004141UL
#define BASIC_HOMEWORK_DROP 0x00006141UL
/*作业发布模块*/
#define SEND_HOMEWORK_SEND 0x00002241UL
#define SEND_HOMEWORK_AMEND 0x00004241
#define SEND_HOMEWORK_DELETE 0x00006241UL
/*教师作业查看模块*/
#define TEACHER_VIEW_SUBMIT_HOMEWORK 0x00002341UL
#define TEACHER_VIEW_SEND_HOMEWORK 0x00004341UL
#define TEACHER_VIEW_HOMEWORK_CONTENT 0x00006341UL
/*作业批改模块*/
#define CHECK_HOMEWORK_SUBJECT 0x00002441UL
#define CHECK_HOMEWORK_SET_SUBJECT_SCORE 0x00004441UL
/*学生作业查看模块*/
#define STUDENT_VIEW_HOMEWORK_LIST 0x00002541UL
/*学生作业测试模块*/
#define GET_HOMEWORK_CONTENT 0x00002641UL
#define HANDIN_HOMEWORK_REPORT 0x00004641UL
/*作业情况查看模块*/
#define VIEW_HOMEWORK_CLASS_HANDIN 0x00002741UL
#define VIEW_HOMEWORK_STUDENT_HANDIN 0x00004741UL
#define VIEW_HOMEWORK_CLASS_SCORE_RANK 0x00006741UL
#define VIEW_HOMEWORK_STUDENT_SCORE_RANK 0x00008741UL
#define VIEW_HOMEWORK_REPORT 0x0000A741UL

//typedef int SOCKET;

using namespace std;
using namespace edwin;
struct User
{
	string userIP;
	unsigned int userID;
};
struct Class
{
	list<User> students;
	unsigned int classID;
	unsigned int class_manager;
};
struct Member
{
	unsigned int memberID;
	string password;
};
template <class T,class R>
struct Task
{
	Task(function<R (T)> t_func, T t_arg):func(t_func),arg(t_arg){};
	Task(){};
	function<R (T)> func;
	T arg;
};
struct UserSockInfo
{
	int sockfd;
	string remoteIP;
	int remote_port;
};
struct UserInfo
{
	int sockfd;
	string remoteIP;
	int remote_port;
	string info;
};
struct Protocol
{
	unsigned int message_type;
	unsigned int subsystem;
	unsigned int module;
	unsigned int module_operate;
	string content;
};
/*全局变量*/
extern ConnectionPool homework_cpool;
extern Tsocket file_socket;
extern list<Class> class_list;
extern list<Member> user_list;
extern list<UserSockInfo> accept_all_sockfd;
extern list<UserInfo> all_user_info;
extern list<UserInfo> send_udp_info;
extern list<UserInfo> send_tcp_info;
extern mutex user_sock_info_mutex;
extern mutex user_info_mutex;
extern mutex send_udp_info_mutex;
extern mutex send_tcp_info_mutex;
extern mutex accept_all_sockfd_mutex;
extern mutex all_user_info_mutex;
extern condition_variable accept_all_sockfd_cond;
extern condition_variable all_user_info_cond;
extern mutex send_udp_mutex;
extern mutex send_tcp_mutex;
extern condition_variable send_udp_cond;
extern condition_variable send_tcp_cond;
/*调用函数*/
extern bool AddBasicHomework(unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath,unsigned int title_num);
extern int AmendBasicHomework(unsigned int homeworkid,unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath, unsigned int title_num, int change_item);
extern bool DeleteBasicHomework(unsigned int homeworkid);
extern bool AddPublishHomework(unsigned int homeworkid,string homeworkname,string deadline, unsigned int classid);
extern int AmendPublishHomework(unsigned int homeworkid,string homeworkname, string deadline, unsigned int classid,unsigned int new_classid, int change_item);
extern bool DeletePublishHomework(unsigned int homeworkid,unsigned int classid);
extern string ViewSubmitHomework(unsigned int  teacherid);
extern string ViewPublishHmoework(unsigned int teacherid);
extern bool GetPublishCorrectstate(unsigned int homeworkid,unsigned int teacherid, unsigned int classid);
extern unsigned int  GetStudentFinishNum(unsigned int homeworkid,unsigned int teacherid, unsigned int classid);
extern string ViewStudentHomeworkList(unsigned int studentid,bool homework_state);
extern string GetHomeworkTitle(unsigned int homeworkid,unsigned int studentid);
extern string GetStandardAnswer(unsigned int homeworkid,unsigned int studentid);
extern bool SetStudentAnswer(unsigned int homeworkid,unsigned int studentid,unsigned int classid,string choice_answer,string object_answer,unsigned int choice_score);
extern string GetChoiceStandardAnswer(unsigned int homeworkid);
extern string GetHomeworkPath(unsigned int homeworkid);
extern string GetStudentSubjectContent(unsigned int homeworkid,unsigned int classid);
extern bool  SetStudentSubjectScore(unsigned int homeworkid,unsigned int studentid,unsigned int score,unsigned int scale);
extern string ViewHomeworkClassHandIn(unsigned int classid);
extern string  ViewHomeworkStudentHandIn(unsigned int studentid);
extern string GetReportName(unsigned int homeworkid,unsigned int classid);
extern string ViewHomeworkReport(unsigned int homeworkid,unsigned int classid);
extern string ViewClassScoreRank(unsigned int homeworkid,unsigned int classid);
extern string ViewStudentScoreRank(unsigned int  studentid,unsigned int classid);
extern bool CheckStudentCount(unsigned int homeworkid,unsigned int classid,int count);
#endif
