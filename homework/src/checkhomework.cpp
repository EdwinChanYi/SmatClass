/*
 * checkhomework.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: miller
 */
#include "homework.h"
#include "main.h"
#include "handle.h"
#include <sstream>
using namespace std;
using namespace edwin;
string CheckHomework:: get_subjet_answer()
{
	string content = GetStudentSubjectContent(homeworkid,classid);
	if(content.empty())
	{
		cout<<"nothing subject\n";
	}
	return content;
}
bool CheckHomework::set_subject_score()
{
	list<string> subject_score = Handle::c_to_list(student_score,':');
	list<string>::iterator it = subject_score.begin();
	unsigned int studentid;
	unsigned int score;
	int legth = subject_score.size();
	while(legth--)
	{
		istringstream check_stream(Handle::c_to_c(*(it++),','));
		check_stream>>studentid;
		check_stream>>score;
		 if(!SetStudentSubjectScore(homeworkid,studentid,score,scale))
		 {
			 cout<<"set student subject score error\n";
			 return false;
		 }
	}
	if(!CheckStudentCount(homeworkid,classid,subject_score.size()))
	{
		cout<<"check student count error\n";
		return false;
	}

	 return true;

}



