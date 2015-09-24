/*
 * viewhomework.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: miller
 */
#include "homework.h"
#include "main.h"
#include "file.h"
#include <iostream>
#include "handle.h"
using namespace edwin;
using namespace std;
string ViewTeacherHomework::view_submit()
{
	string view_info = ViewSubmitHomework(teacherid);
	return view_info;
}
string ViewTeacherHomework::view_publish()
{
	string view_info = ViewPublishHmoework(teacherid);
	return view_info;
}
string ViewTeacherHomework::view_homework_content()
{
	string homework_content = GetHomeworkPath(homeworkid);
	File get_file(homework_content, ios::in|ios::out);
	get_file.open(ios::in|ios::out);
	homework_content = get_file.read();
	homework_content = Handle::int_to_string(homeworkid)+"@"+homework_content+"@"+GetChoiceStandardAnswer(homeworkid);
	return homework_content;
}
