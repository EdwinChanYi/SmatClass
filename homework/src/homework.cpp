#include "homework.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sys/io.h>
#include "main.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "handle.h"

using namespace edwin;
using namespace std;
bool SubmitHomework::save_homework()
{
		string path(HOMEPATH);
		path = path+Handle::int_to_string((int)teacherid)+"/"+teacherpath;
		if(access(path.c_str(), 0))
		{
			string commend("mkdir ");
			commend += path;
			system(commend.c_str());
		}
		path = path+"/"+homeworkname+".txt";
		File create_file(path, ios_base::in|ios_base::out);
		create_file.open(ios_base::in|ios_base::out|ios::trunc);
		if(!create_file.write(homeworkcontent))
		{
			cout<<"write error\n";
			return false;
		}
		if(!AddBasicHomework(teacherid, homeworkname, answer, homeworkcontent,teacherpath,title_num))
		{
			cerr<<"add basic-homework error\n";
			return false;
		}

		return true;
	}
bool SubmitHomework::amend_homework(int change_item)
{
	cout<<"amend homeowrk\n";
	if(AmendBasicHomework(homeworkid,teacherid,homeworkname, answer, homeworkcontent,teacherpath,title_num, change_item))
	{
		cout<<"amend homework error\n";
		return false;
	}
	return true;
}
bool SubmitHomework::delete_homework()
{
	if(!DeleteBasicHomework(homeworkid))
	{
		cout<<"delete error\n";
		return false;
	}
	return true;
}
