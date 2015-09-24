#ifndef BASIC_HOMEWORK_H
#define BASIC_HOMEWORK_H
#include <iostream>
#include <string>
#include <list>
using namespace std;
namespace edwin
{
	struct BasicHomework
	{
		unsigned int homeworkid;
		string homeworkname;
		unsigned int teacherid;
		unsigned int title_num;
		BasicHomework(unsigned int h_homeworkid,unsigned int h_teacherid, string h_name,unsigned int h_title_num):homeworkid(h_homeworkid),teacherid(h_teacherid),homeworkname(h_name),title_num(h_title_num){}
		BasicHomework(unsigned int h_homeworkid,string h_homeworkname):homeworkid(h_homeworkid),homeworkname(h_homeworkname){}
		BasicHomework(unsigned int h_homeworkid):homeworkid(h_homeworkid){}
		BasicHomework(){}
		virtual ~BasicHomework(){}
	};
	struct Homework:public BasicHomework
	{
		unsigned int classid;
		string sendtime;
		string deadline;
		bool homework_check_state;
		Homework(unsigned int h_homeworkid,string h_homeworkname,unsigned int h_classid, string h_deadline):BasicHomework(h_homeworkid,h_homeworkname)
		{
			classid = h_classid;
			deadline = h_deadline;
		}
		Homework(unsigned int h_homeworkid):BasicHomework(h_homeworkid){}
		Homework(){}
		virtual ~Homework(){}
	};
	class SubmitHomework:public BasicHomework  
	{
	public:
		string teacherpath;
		string answer;
		string homeworkcontent;
	    SubmitHomework(string path,string ans, string content, BasicHomework &basichomework):teacherpath(path),answer(ans),homeworkcontent(content),BasicHomework(basichomework){}
	    SubmitHomework(){}
	    virtual ~SubmitHomework(){}
		//保存作业
		bool save_homework();
		//修改作业
		bool amend_homework(int change_item);
		 
		//删除作业
		bool delete_homework();  
	};
	class PublishHomework:public Homework
	{
	public:
		unsigned int new_classid;
		PublishHomework(Homework homework,unsigned int h_new_class):Homework(homework)
		{
			new_classid = h_new_class;
		}
		PublishHomework(){}
		PublishHomework(Homework homework):Homework(homework){}
		~PublishHomework(){}
		bool publish_homework();
		bool amend_homework(int change_item);
		bool delete_homework();
	};

	class ViewTeacherHomework
	{
	public:
		unsigned int teacherid;
		unsigned int homeworkid;
		ViewTeacherHomework(){}
		ViewTeacherHomework(unsigned int h_teacherid):teacherid(h_teacherid){}
		ViewTeacherHomework(unsigned int h_teacherid,unsigned int h_homeworkid):teacherid(h_teacherid),homeworkid(h_homeworkid){}
		~ViewTeacherHomework(){}
		string view_submit();
		string view_publish();
		string view_homework_content();
	};
	class ViewStudentHomework
	{
	public:
		unsigned int studentid;
		bool homework_state;
		ViewStudentHomework(){}
		ViewStudentHomework(unsigned int h_studentid,bool h_homework_state):studentid(h_studentid),homework_state(h_homework_state){}
		~ViewStudentHomework(){}
		string show_list();
	};
	class DoHomework:public Homework
	{
	public:
		unsigned int studentid;
		string choice_answer;
		string object_answer;
		unsigned int classid;
		DoHomework(unsigned int h_studentid,unsigned int h_homework):Homework(h_homework)
		{
			studentid = h_studentid;
		}
		DoHomework(){}
		DoHomework(unsigned int h_studentid,unsigned int h_homework,unsigned int h_classid,string h_choice_answer,string h_object_answer):Homework(h_homework)
		{
			studentid = h_studentid;
			choice_answer = h_choice_answer;
			object_answer = h_object_answer;
			classid = h_classid;
		}
		~DoHomework(){}
		string get_homework();
		bool hand_in_homework();
	};

	class CheckHomework
	{
	public:
		unsigned int homeworkid;
		unsigned int classid;
		string student_score;
		unsigned int scale;
		CheckHomework(unsigned int h_homeworkid,unsigned int h_classid,string h_student_score,unsigned int h_scale):homeworkid(h_homeworkid),classid(h_classid),student_score(h_student_score),scale(h_scale){}
		CheckHomework(){}
		~CheckHomework(){}
		string get_subjet_answer();
		bool set_subject_score();
	};
	class ViewHomeworkSituation
	{
	public:
		unsigned int homeworkid;
		unsigned int studentid;
		unsigned int teacherid;
		unsigned int classid;

		ViewHomeworkSituation(){}
		~ViewHomeworkSituation(){}
		string show_class_hand_in();
		string show_student_hand_in();
		string show_homework_report_situation();
		string show_class_score_rank();
		string show_student_score_rank();
	};
}
#endif
