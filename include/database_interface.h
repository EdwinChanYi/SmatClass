#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include <string>
#include <iostream>
#include "homework.h"

using namespace std;
using namespace edwin;
extern bool AddBasicHomework(unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath);
extern int AmendBasicHomework(unsigned int homeworkid,unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath, int change_item);
extern bool DeleteBasicHomework(unsigned int homeworkid);
extern bool AddPublishHomework(Homework &homework);
extern string AmendPublishHomework(Homework &homework);
extern bool DeletePublishHomework(unsigned int homeworkid,unsigned int classid);
extern string ViewSubmitHomework(unsigned int  teacherid);
extern string ViewPublishHmoework(unsigned int teacherid,unsigned int classid);
extern bool GetPublishCorrectstate(unsigned int homeworkid,unsigned int teacherid, unsigned int classid);
extern unsigned int  GetStudentFinishNum(unsigned int homeworkid,unsigned int teacherid, unsigned int classid);
#endif
