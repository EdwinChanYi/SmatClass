/*
 * sendhomework.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: miller
 */
#include "homework.h"
#include "main.h"
using namespace edwin;

bool PublishHomework::publish_homework()
{
	if(!AddPublishHomework(homeworkid,homeworkname,deadline,classid))
	{
		cout<<"publish homework error\n";
		return false;
	}
	return true;
}
bool PublishHomework::amend_homework(int change_item)
{
	if(AmendPublishHomework(homeworkid,homeworkname,deadline,classid, new_classid,change_item))
	{
		cout<<"amend publish homework error\n";
		return false;
	}
	return true;
}
bool PublishHomework::delete_homework()
{
	if(!DeletePublishHomework(homeworkid,classid))
	{
		cout<< "delete publish homework error\n";
		return false;
	}
	return true;

}

