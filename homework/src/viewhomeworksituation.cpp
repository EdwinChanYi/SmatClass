/*
 * viewhomeworksituation.cpp
 *
 *  Created on: 2015年7月31日
 *      Author: miller
 */

#include "homework.h"
#include "main.h"
#include "handle.h"
#include "file.h"
using namespace edwin;

string ViewHomeworkSituation::show_class_hand_in()
{
	string view = ViewHomeworkClassHandIn(classid);
	if(view.empty())
		cout<<"view class hand in error\n";
	view = Handle::int_to_string(classid)+view;
	return view;
}
string ViewHomeworkSituation::show_student_hand_in()
{
	string view = ViewHomeworkStudentHandIn(studentid);
	view = Handle::int_to_string(studentid)+view;
	return view;
}
string ViewHomeworkSituation::show_homework_report_situation()
{
	string report_path = ViewHomeworkReport(homeworkid,classid);
	File report_file(report_path,ios::in|ios::out);
	report_file.open(ios::in|ios::out);
	string report_content = report_file.read();
	string report_name = GetReportName(homeworkid,classid);
	report_content = report_name+":"+report_content;
	return report_content;
}
string ViewHomeworkSituation::show_class_score_rank()
{
	string return_data = ViewClassScoreRank(homeworkid,classid);
	return_data = GetReportName(homeworkid,classid)+":"+return_data;
	return return_data;

}
string ViewHomeworkSituation::show_student_score_rank()
{
	string str =  ViewStudentScoreRank(studentid,classid);
	*(str.end()-1) = '\0';
	return str;
}
