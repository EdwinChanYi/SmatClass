/*
 * dohomework.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: miller
 */
#include "homework.h"
#include <iostream>
#include "main.h"
#include <string>
#include "file.h"
#include "handle.h"
#include <sstream>
using namespace std;
using namespace edwin;

string DoHomework::get_homework()
{
	string get_content = GetHomeworkTitle(homeworkid,studentid);
	cout<<get_content<<endl;
	File get_file(get_content, ios::in|ios::out);
	get_file.open(ios::in|ios::out);
	get_content = get_file.read();
	string standard = GetStandardAnswer(homeworkid,studentid);
	get_content = Handle::int_to_string(homeworkid)+"#0"+get_content+standard;
	return get_content;
}
bool DoHomework::hand_in_homework()
{
	unsigned int choice_score = 0;
	string standard_answer = GetChoiceStandardAnswer(homeworkid);
	if(standard_answer == string())
	{
		cout<<"get answer error\n";
		return false;
	}
	list<string> stu = edwin::Handle::c_to_list(choice_answer,',');
	list<string> standard = edwin::Handle::c_to_list(standard_answer,',');
	while(!stu.empty()&&!standard.empty())
	{
		cout<<"correct\n";

		if(!stu.front().compare(standard.front()))
			choice_score++;
		stu.pop_front();
		standard.pop_front();
		cout<<choice_score<<endl;
	}
	cout<<"out\n";
	if(!SetStudentAnswer(homeworkid,studentid,classid,choice_answer,object_answer,choice_score))
	{
		cout<<"set student answer\n";
		return false;
	}
	return true;
}



