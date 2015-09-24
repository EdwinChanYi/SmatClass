/*
 * viewstudenthomework.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: miller
 */
#include "homework.h"
#include "main.h"
using namespace edwin;

string ViewStudentHomework::show_list()
{
	string view_stu_homework = ViewStudentHomeworkList(studentid,homework_state);
	return view_stu_homework;
}

