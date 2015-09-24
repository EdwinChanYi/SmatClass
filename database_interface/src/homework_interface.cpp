#include <string>
#include "homework.h"
#include "mysqloperation.h"
#include "file.h"
#include "main.h"
#include "handle.h"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <sstream>
#define NAME_CHANGE 0
#define CONTENT_CHANGE 1
#define PATH_CHANGE 2
#define DEADLINE_CHANGE 1
#define ANSWER_CHANGE 3
#define CLASS_CHANGE 2
using namespace edwin;
using namespace std;
/*保存作业，用到教师号，作业名，标准答案，作业内容，教师路径，作业行数*/
bool AddBasicHomework(unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath,unsigned int title_num)
{
	
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query("insert into Homework(homeworkname,teacherID,homeworkpath,homeworkanswer,titlenum) values(");
	query = query+"'"+homeworkname+"'"+","+edwin::Handle::int_to_string(teacherid)+","+"'"+HOMEPATH+edwin::Handle::int_to_string(teacherid)+"/"+teacherpath+"'"+","+"'"+answer+"',"+edwin::Handle::int_to_string(title_num)+");";
	cout<<query<<endl;
	if(mysql->insert(query.c_str()))
	{
		homework_cpool.release_connection(mysql);
		return false;
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*修改作业，用到作业号，教师号，修改的作业名，作业答案，作业内容，作业路径，作业选择题数。修改的标志*/
int AmendBasicHomework(unsigned int homeworkid,unsigned int teacherid,string homeworkname, string answer, string homeworkcontent,string teacherpath,unsigned int title_num, int change_item)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string path;
	string beforepath;
	string commend;
	int i = 0; 
	while(i<4)
	{
		cout<<"amend basic homework\n";
		cout<<(change_item&(1<<i))<<endl;
		if((change_item&(1<<i)) != 0)
		{
			cout<<i<<endl;
			switch(i)
			{
				case NAME_CHANGE:
					query = string("select homeworkpath,homeworkname from Homework where homeworkID=");
					query = query+edwin::Handle::int_to_string(homeworkid)+";";
					cout<<query<<endl;
					if(mysql->select(query.c_str()))
					{
						cout<<"select homeworkname error\n";
						homework_cpool.release_connection(mysql);
						return -2;
					}
					beforepath = beforepath+mysql->res[0][0]+"/"+mysql->res[0][1]+".txt";
					path =path+mysql->res[0][0]+"/"+homeworkname+".txt";
					cout<<beforepath<<endl;
					cout<<path<<endl;
					if(access(beforepath.c_str(), 0))
					{
						commend = "touch "+path;
						system(commend.c_str());
					}
					else
					{
						commend = "mv "+beforepath+" "+path;
						system(commend.c_str());
					}
					query = string("update Homework set homeworkname=");
					query = query+"'"+homeworkname+"'"+"where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+";";
					cout<<query<<endl;
					if(mysql->update(query.c_str()))
					{
						cout<<"update homeworkname error\n";
						homework_cpool.release_connection(mysql);
						return -1;
					}
					cout<<"change name\n";
					break;
				case CONTENT_CHANGE:
					query = string("select homeworkpath,homeworkname from Homework where homeworkID=");
					query = query+edwin::Handle::int_to_string(homeworkid)+";";
					cout<<query<<endl;
					if(mysql->select(query.c_str()))
					{
						cout<<"select homeworkname error\n";
						homework_cpool.release_connection(mysql);
						return -2;
					}
					path = path+mysql->res[0][0]+"/"+mysql->res[0][1]+".txt";
					cout<<path<<endl;
					cout<<"change content\n";
					break;
				case PATH_CHANGE:
					query = string("select homeworkpath from Homework where homeworkID=")+edwin::Handle::int_to_string(homeworkid)+";";
					cout<<query<<endl;
					cout<<"path change\n";
					if(mysql->select(query.c_str()))
					{
						cout<<"select homeworkpath error\n";
						homework_cpool.release_connection(mysql);
						return -2;
					}
					beforepath += mysql->res[0][0];
					cout<<beforepath<<endl;
					path = HOMEPATH;
					path = path+edwin::Handle::int_to_string(teacherid)+"/"+teacherpath;
					if(access(path.c_str(), 0))
					{
						commend = "mkdir "+path;
						system(commend.c_str());
					}
					else
					{
						commend = "mv "+beforepath+" "+path+" -r";
						if(system(commend.c_str())<=0)
						{
							commend = "mkdir "+path;
							system(commend.c_str());
						}

					}
					query = string("update Homework set homeworkpath=");
				    query = query+"'"+HOMEPATH+edwin::Handle::int_to_string(teacherid)+"/"+teacherpath+"' "+"where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+";";
				    cout<<query<<endl;
					if(mysql->update(query.c_str()))
					{
						cout<<"update homeworkpath error\n";
						homework_cpool.release_connection(mysql);
						return -3;
					}
					cout<<"change path\n";
					break;
				case ANSWER_CHANGE:
					query = string("update Homework set homeworkanswer=");
					query = query+"'"+answer+"' "+"where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+";";
					cout<<query<<endl;
					if(mysql->update(query.c_str()))
					{
						cout<<"update homeworkanswer error\n";
						homework_cpool.release_connection(mysql);
						return -4;
					}
					cout<<"change answer\n";
					break;
				default:
					break;
			}
			if(i == 1)
			{
				File fd(path, ios_base::in|ios_base::out|ios_base::trunc);
				fd.open(ios_base::in|ios_base::out|ios_base::trunc);
				fd.write(homeworkcontent);
			}
		}
		i++;
	}
	homework_cpool.release_connection(mysql);
	return 0;
}
/*删除作业，用到作业号*/
bool DeleteBasicHomework(unsigned int homeworkid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string commad;
	query = "select homeworkpath, homeworkname from Homework where homeworkID="+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select homeworkpath error\n";
		homework_cpool.release_connection(mysql);
		return -2;
	}
	string path;
	path = path+mysql->res[0][0]+"/"+mysql->res[0][1]+".txt";
	cout<<path<<endl;
	if(!access(path.c_str(),0))
	{
		commad = commad+"rm "+path;
		if(system(commad.c_str())>0)
		{
			cout<<"can't delete file\n";
			return false;
		}
	}
	cout<<commad<<endl;
	query = string();
	query = "delete from Homework where homeworkID="+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->drop(query.c_str()))
	{
		cout<<"delete basic homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*发布作业，作业号，作业名，截止时间，班级号*/
bool AddPublishHomework(unsigned int homeworkid,string homeworkname,string deadline, unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query("select titlenum from Homework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select titlenum error\n";
		homework_cpool.release_connection(mysql);
		return -2;
	}
	string create("create table ");
	string table_name = edwin::Handle::int_to_string(homeworkid)+homeworkname+edwin::Handle::int_to_string(classid);
	create = create+edwin::Handle::int_to_string(homeworkid)+homeworkname+edwin::Handle::int_to_string(classid)+"(title smallint unsigned,A smallint unsigned, B smallint unsigned, C smallint unsigned,D smallint unsigned);";
	cout<<create<<endl;
	if(mysql->create_table(create.c_str()))
	{
		homework_cpool.release_connection(mysql);
		return false;
	}
	for(int i=1; i<=atoi(mysql->res[0][0]);i++)
	{
		create = "insert into "+table_name+" values(";
		create = create+edwin::Handle::int_to_string(i)+","+edwin::Handle::int_to_string(0)+","+edwin::Handle::int_to_string(0)+","+edwin::Handle::int_to_string(0)+","+edwin::Handle::int_to_string(0)+");";
		cout<<create<<endl;
		if(mysql->insert(create.c_str()))
		{
			homework_cpool.release_connection(mysql);
			return false;
		}
	}
	query = string("insert into SendHomework(homeworkID,homeworkname, sendtime,deadline,classID,homeworkstate,feedbacktable) values(");
	query = query+edwin::Handle::int_to_string(homeworkid)+","+"'"+homeworkname+"'"+","+"UNIX_TIMESTAMP(now())"+","+"UNIX_TIMESTAMP('"+deadline+"'), "+edwin::Handle::int_to_string(classid)+","+edwin::Handle::int_to_string(0)+",'"+table_name+"');";
	cout<<query<<endl;
	if(mysql->insert(query.c_str()))
	{
		homework_cpool.release_connection(mysql);
		return false;
	}
	query = string("insert into StudentHomework(homeworkID,classID,studentID) select s.homeworkID,s.classID,d.memberID from SendHomework s, (select m.memberID memberID,c.classID classID from Member m,ClassMember c where m.membertype=0 and c.classID=");
	query = query+edwin::Handle::int_to_string(classid)+ " and m.memberID=c.memberID) d where s.homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and s.classID=d.classID;";

	cout<<query<<endl;
	if(mysql->insert(query.c_str()))
	{
		homework_cpool.release_connection(mysql);
		return false;
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*修改发布作业*/
int AmendPublishHomework(unsigned int homeworkid,string homeworkname, string deadline, unsigned int classid, unsigned int new_classid,int change_item)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string path;
	string beforepath;
	string commend;
	int i = 0;
	while(i<3)
	{
		cout<<"amend basic homework\n";
		cout<<(change_item&(1<<i))<<endl;
		if((change_item&(1<<i)) != 0)
		{
			cout<<i<<endl;
			switch(i)
			{
				case NAME_CHANGE:

					query = string("update SendHomework set homeworkname=");
					query = query+"'"+homeworkname+"'"+"where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+";";
					cout<<query<<endl;
					if(mysql->update(query.c_str()))
					{
						cout<<"update homeworkname error\n";
						homework_cpool.release_connection(mysql);
						return -1;
					}
						cout<<"change name\n";
						break;

					case DEADLINE_CHANGE:

						query = string("update SendHomework set deadline=");
					    query = query+"UNIX_TIMESTAMP('"+deadline+"') "+"where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+";";
					    cout<<query<<endl;
						if(mysql->update(query.c_str()))
						{
							cout<<"update deadline error\n";
							homework_cpool.release_connection(mysql);
							return -3;
						}
						cout<<"change path\n";
						break;
					case CLASS_CHANGE:
						query = string("update SendHomework set classID=");
						query = query+edwin::Handle::int_to_string(new_classid)+" where "+"homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+";";
						cout<<query<<endl;
						if(mysql->update(query.c_str()))
						{
							cout<<"update homeworkanswer error\n";
							homework_cpool.release_connection(mysql);
							return -4;
						}
						cout<<"change answer\n";
						break;
					default:
						break;
				}

			}
			i++;
		}
		homework_cpool.release_connection(mysql);
		return 0;
}
/*删除发布作业*/
bool DeletePublishHomework(unsigned int homeworkid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
		string query;
		string commad;
		query = "select feedbacktable from SendHomework where homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+";";
		cout<<query<<endl;
		if(mysql->select(query.c_str()))
		{
			cout<<"select feedbacktable error\n";
			homework_cpool.release_connection(mysql);
			return -2;
		}
		string table = mysql->res[0][0];
		cout<<table<<endl;
		query = "drop table "+table+";";
		cout<<query<<endl;
		if(mysql->drop_table(query.c_str()))
		{
			cout<<"delete basic homework error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
		query = "delete from SendHomework where homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+";";
		cout<<query<<endl;
		if(mysql->drop(query.c_str()))
		{
			cout<<"delete send homework error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
		homework_cpool.release_connection(mysql);
		return true;
}
/*教师查看提交作业*/
string ViewSubmitHomework(unsigned int  teacherid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string view_info;
	string temp_str;
	string main_path = HOMEPATH;

	query = string("select homeworkID,homeworkname,homeworkpath from Homework where teacherID=");
	query = query+edwin::Handle::int_to_string(teacherid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	for(int i = 0; i<mysql->line_num;i++)
	{
		for(int j = 0;j<3; j++)
		{
			if(j<2)
				temp_str = mysql->res[i][j];
			else
			{
				temp_str = mysql->res[i][j];
				temp_str.erase(0,main_path.size());
			}

			view_info = view_info+temp_str;
			if(j<2)
				view_info += ",";
		}
		if(i<mysql->line_num-1)
			view_info += ":";
	}
	cout<<view_info<<endl;
	homework_cpool.release_connection(mysql);
	return view_info;
}
/*教师查看发布作业*/
string ViewPublishHmoework(unsigned int teacherid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string view_info;

	query = string("select distinct c.classID,c.classname,d.homeworkID,d.homeworkname,FROM_UNIXTIME(d.deadline) from ClassMember c, (select s.classID classID, h.homeworkID homeworkID,h.homeworkname homeworkname,s.deadline deadline from Homework h,SendHomework s where h.teacherID = ");
	query = query+edwin::Handle::int_to_string(teacherid)+" and h.homeworkID=s.homeworkID) d where c.classID=d.classID;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	for(int i = 0; i<(mysql->line_num);i++)
	{
			for(int j = 0;j<5; j++)
			{
				if(mysql->res[i][j] != NULL)
					view_info = view_info+mysql->res[i][j];
				else
					view_info = view_info+" ";
				if(j<4)
					view_info += ",";
			}

			if(i<(mysql->line_num-1))
				view_info += "\\";
	}
		cout<<view_info<<endl;
		homework_cpool.release_connection(mysql);
		return view_info;
}
/*学生查看作业列表*/
string ViewStudentHomeworkList(unsigned int studentid,bool homework_state)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	string view_info;

	query = string("select se.homeworkID,se.homeworkname from SendHomework se,(select homeworkID,classID from StudentHomework where studentID=");
	query = query+edwin::Handle::int_to_string(studentid)+" and homeworkstate="+edwin::Handle::int_to_string((unsigned int)homework_state)+") d where d.homeworkID = se.homeworkID and d.classID=se.classID;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	for(int i = 0; i<(mysql->line_num);i++)
	{
		for(int j = 0;j<2; j++)
		{
			if(mysql->res[i][j] != NULL)
				view_info = view_info+mysql->res[i][j];
			if(j<1)
				view_info += ",";
		}

		if(i<(mysql->line_num-1))
			view_info += ":";
	}
	cout<<view_info<<endl;
	homework_cpool.release_connection(mysql);
	return view_info;
}
/*获取作业存储路径*/
string GetHomeworkPath(unsigned int homeworkid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string("select homeworkpath,homeworkname from Homework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string(mysql->res[0][0]);
	query = query+"/"+mysql->res[0][1]+".txt";
	homework_cpool.release_connection(mysql);
	return query;
}
/*获取学生主观题答案*/
string GetStudentSubjectContent(unsigned int homeworkid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string("select studentID,subjectitemanswer from StudentHomework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+" and classID="+edwin::Handle::int_to_string(classid)+" and homeworkstate=1;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string();
	for(int i = 0;i<mysql->line_num;i++)
	{
		for(int j = 0;j<2;j++)
		{
			if(mysql->res[i][j] != NULL)
				 query = query+mysql->res[i][j];
			 else
				query = query+" ";

			if(j==0)
				query = query+"@";
		}
		if(i<mysql->line_num-1)
		{
			query = query+"\\";
		}
	}
	cout<<query<<endl;
	return query;
}
/*设置学生主管题分数及计算总分*/
bool  SetStudentSubjectScore(unsigned int homeworkid,unsigned int studentid,unsigned int score,unsigned int scale)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	int choice_score = 0;
	query = string("select choicescore from StudentHomework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+" and studentID="+Handle::int_to_string(studentid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	if(mysql->res[0][0] != NULL)
	{
		choice_score = atoi(mysql->res[0][0])*scale;
	}
	mysql->free_result();
	query = string("update StudentHomework set subjectscore=");
	query = query+edwin::Handle::int_to_string(score)+",checkstate=1,totalscore="+Handle::int_to_string(score+choice_score)+" where homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and studentID="+edwin::Handle::int_to_string(studentid)+";";
	cout<<query<<endl;
	if(mysql->update(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*检查作业是否批改完*/
bool CheckStudentCount(unsigned int homeworkid,unsigned int classid,int count)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	int num;
	query = string("select count(studentID) from StudentHomework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+" and homeworkstate=1 group by studentID;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	num = atoi(mysql->res[0][0]);
	mysql->free_result();
	query = string("update SendHomework set checknum=");
	query = query+edwin::Handle::int_to_string(num-count)+" where homeworkID="+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->update(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	cout<<(num-count)<<endl;
	if(num-count <= 0)
	{
		query = string("update SendHomework set homeworkstate=1  where homeworkID=");
		query = query+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+";";
		cout<<query<<endl;
		if(mysql->update(query.c_str()))
		{
			cout<<"select view homework error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
		query = string("select feedbacktable,titlenum,homeworkpath from SendHomework s,Homework h where s.homeworkID=");
		query = query+edwin::Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+" and h.homeworkID=s.homeworkID;";
		cout<<query<<endl;
		if(mysql->select(query.c_str()))
		{
			cout<<"select view homework error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
		string feedback_table = mysql->res[0][0];
		unsigned int title_num = atoi(mysql->res[0][1]);
		string feedback_path = mysql->res[0][2];

		feedback_path = feedback_path + "/feedback";
		if(access(feedback_path.c_str(), 0))
		{
			string commend("mkdir ");
			commend += feedback_path;
			system(commend.c_str());
		}
		feedback_path = feedback_path + "/"+feedback_table+".txt";
		if(access(feedback_path.c_str(), 0))
		{
			string commend("touch ");
			commend += feedback_path;
			system(commend.c_str());
		}
		File feedback_file(feedback_path,ios::in|ios::out|ios::app|ios::ate);
		feedback_file.open(ios::in|ios::out|ios::app|ios::ate);
		string write_file("title\tA\tB\tC\tD");
		if(feedback_file.write_row(write_file) == -1)
		{
			cout<<"write error\n";
			return false;
		}
		mysql->free_result();
		for(int i = 1;i<=title_num;i++)
		{
			query = string("select A,B,C,D from ");
			query = query+feedback_table+" where title="+Handle::int_to_string(i)+" ;";
			cout<<query<<endl;
			if(mysql->select(query.c_str()))
			{
				cout<<"select feeback table error\n";
				homework_cpool.release_connection(mysql);
				return false;
			}
			write_file = Handle::int_to_string(i);
			write_file = write_file+"\t"+mysql->res[0][0]+"\t"+mysql->res[0][1]+"\t"+mysql->res[0][2]+"\t"+mysql->res[0][3];
			if(feedback_file.write_row(write_file) == -1)
			{
				cout<<"write error\n";
				return false;
			}
			mysql->free_result();
		}
		query = string("update SendHomework set feedbackpath='");
		query = query+ feedback_path+"'  where homeworkID="+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+";";
		cout<<query<<endl;
		if(mysql->update(query.c_str()))
		{
			cout<<"select view homework error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*获取作业内容*/
string GetHomeworkTitle(unsigned int homeworkid,unsigned int studentid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;

	query = string("select homeworkpath,homeworkname from Homework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string(mysql->res[0][0]);
	query = query+"/"+mysql->res[0][1]+".txt";
	homework_cpool.release_connection(mysql);
	return query;
}
/*判断学生是否完成作业，学生获取选择题标准答案*/
string GetStandardAnswer(unsigned int homeworkid,unsigned int studentid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string("select homeworkstate from StudentHomework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+" and studentID="+edwin::Handle::int_to_string(studentid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	if(!strcmp(mysql->res[0][0],"\0"))
	{
		mysql->free_result();
		return string();
	}
	mysql->free_result();
	query = string("select homeworkanswer from Homework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	mysql->free_result();
	homework_cpool.release_connection(mysql);
	query = string("\\");
	query = query+mysql->res[0][0];
	return query;
}
/*获取选择题标准答案*/
string GetChoiceStandardAnswer(unsigned int homeworkid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string("select homeworkanswer from Homework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homework error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string(mysql->res[0][0]);
	homework_cpool.release_connection(mysql);
	return query;
}
/*设置学生答案*/
bool SetStudentAnswer(unsigned int homeworkid,unsigned int studentid,unsigned int classid,string choice_answer,string object_answer,unsigned int choice_score)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;

	query = string("update StudentHomework set choiceanswer=");
	query = query+"'"+choice_answer.c_str()+"',subjectitemanswer='"+object_answer.c_str()+"',choicescore="+edwin::Handle::int_to_string(choice_score)+" where homeworkID="+edwin::Handle::int_to_string(homeworkid)+" and studentID="+edwin::Handle::int_to_string(studentid)+";";
	cout<<query<<endl;
	if(mysql->update(query.c_str()))
	{
		cout<<"update student homework error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	string feedback_table;
	query = string("select feedbacktable from SendHomework where homeworkID=");
	query = query+edwin::Handle::int_to_string(homeworkid)+" and classid="+edwin::Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select feedback table error\n";
		homework_cpool.release_connection(mysql);
		return false;
	}
	feedback_table = mysql->res[0][0];
	mysql->free_result();
	istringstream input(choice_answer);
	string column;
	int char_num = choice_answer.size();
	unsigned int title_id = 0;
	while(char_num>0)
	{
		title_id++;
		input>>column;
		char_num =char_num - column.size()-1;
		query = string("update ");
		query = query+feedback_table+" set "+column+"="+column+"+1"+" where title="+Handle::int_to_string(title_id)+";";
		cout<<query<<endl;
		if(mysql->update(query.c_str()))
		{
			cout<<"select update feedback table error\n";
			homework_cpool.release_connection(mysql);
			return false;
		}
	}
	homework_cpool.release_connection(mysql);
	return true;
}
/*查看班级作业提交情况*/
string ViewHomeworkClassHandIn(unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	list<string> homework_list;
	string return_data;
	string temp;
	int not_num = 0;
	query = string("select homeworkid  from SendHomework where classID=");
	query = query+edwin::Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	for(int i = 0;i<mysql->line_num;i++)
	{
		homework_list.push_back( string(mysql->res[i][0]));
	}
	mysql->free_result();
	cout<<homework_list.size()<<endl;
	while(homework_list.size())
	{
		not_num = 0;
		query = string("select m.membername,d.homeworkstate  from Member m,(select studentID,homeworkstate  from StudentHomework where classID=");
		query = query+edwin::Handle::int_to_string(classid)+" and homeworkID="+homework_list.front()+") d where m.memberID=d.studentID"+";";
		return_data = return_data+":"+homework_list.front()+",";
		homework_list.pop_front();
		cout<<query<<endl;
		if(mysql->select(query.c_str()))
		{
			cout<<"select view studentID error\n";
			homework_cpool.release_connection(mysql);
			return string();
		}

		for(int i = 0;i<mysql->line_num;i++)
		{
				if(mysql->res[i][1] != NULL)
				{
					if(strcmp(mysql->res[i][1]," "))
							 not_num++;
				}
				if(mysql->res[i][0] != NULL)
					return_data = return_data+mysql->res[i][0];
				else
					return_data = return_data+" ";
				return_data = return_data+",";
				cout<<mysql->res[i][0]<<endl;
		}
		return_data = return_data+Handle::int_to_string(not_num);
	}

	homework_cpool.release_connection(mysql);
	return return_data;
}
/*查看学生各科目上交情况*/
string ViewHomeworkStudentHandIn(unsigned int studentid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	list<string> homework_list;
	string return_data;
	string temp;
	int not_num = 0;
	query = string(" select membername,count(homeworkstate),homeworkstate from Homework h,(select s.homeworkstate homeworkstate,s.homeworkID homeworkID from StudentHomework  s,SendHomework h where s.studentID=");
	query = query+Handle::int_to_string(studentid)+" and h.homeworkID=s.homeworkID and h.classID = s.classID)  d,Member m where h.homeworkID=d.homeworkID and h.teacherID=m.memberID group by homeworkstate,membername;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string();
	for(int i = 0;i<mysql->line_num;i++)
	{
		if( !strcmp(mysql->res[i][2],"\0"))
				query = query+":";
		for(int j = 0;j<2;j++)
		{
			if(mysql->res[i][j]!=NULL&&!strcmp(mysql->res[i][2],"\0"))
			{
				query = query+mysql->res[i][j];
				if(j == 0)
					query = query+",";
			}
		}
	}
	homework_cpool.release_connection(mysql);
	return query;

}
/*获取作业名跟班级名*/
string GetReportName(unsigned int homeworkid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string(" select homeworkname,classname from SendHomework s,Class c where s.homeworkID=");
	query = query+Handle::int_to_string(homeworkid)+" and c.classID="+Handle::int_to_string(classid)+" and s.classID=c.classID;";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	string return_data = string(mysql->res[0][0]);
	return_data = return_data + ","+Handle::int_to_string(homeworkid)+","+mysql->res[0][1]+","+Handle::int_to_string(classid);
	return return_data;
}
/*获取作业反馈存储路径*/
string ViewHomeworkReport(unsigned int homeworkid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string(" select feedbackpath from SendHomework where homeworkID=");
	query = query+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	query = string(mysql->res[0][0]);
	homework_cpool.release_connection(mysql);
	return query;
}
/*查看班级成绩及排名*/
string ViewClassScoreRank(unsigned int homeworkid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	query = string(" select homeworkstate from SendHomework where homeworkID=");
	query = query+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	if(!strcmp(mysql->res[0][0],"\0"))
	{
		mysql->free_result();
		query = string("select  d.studentID,d.membername,d.totalscore,@rownum := @rownum+1 from (select studentID,membername,totalscore from StudentHomework s,Member m where homeworkID=");
		query = query+Handle::int_to_string(homeworkid)+" and classID="+Handle::int_to_string(classid)+" and m.MemberID=s.studentID) d,(select @rownum:=0) r order by d.totalscore desc";
		cout<<query<<endl;
		if(mysql->select(query.c_str()))
		{
			cout<<"select view homeworkid error\n";
			homework_cpool.release_connection(mysql);
			return string();
		}
		cout<<mysql->line_num<<endl;
		query = string();
		for(int i = 0;i<mysql->line_num;i++)
		{
				query = query+ mysql->res[i][0]+","+mysql->res[i][1]+","+mysql->res[i][2]+","+mysql->res[i][3];
				if(i<mysql->line_num-1)
				{
					query = query+":";
				}
		}
	}
	else
	{
		query = string("have not check");
	}
	homework_cpool.release_connection(mysql);
	return query;
}
/*查看学生班级成绩及排名*/
string ViewStudentScoreRank(unsigned int  studentid,unsigned int classid)
{
	MysqlOperation *mysql = homework_cpool.get_connection();
	string query;
	list<string> homework_list;
	query = string(" select homeworkID from StudentHomework where studentID=");
	query = query+Handle::int_to_string(studentid)+" and classID="+Handle::int_to_string(classid)+";";
	cout<<query<<endl;
	if(mysql->select(query.c_str()))
	{
		cout<<"select view homeworkid error\n";
		homework_cpool.release_connection(mysql);
		return string();
	}
	for(int i = 0;i<mysql->line_num;i++)
	{
		homework_list.push_back(mysql->res[i][0]);
	}
	mysql->free_result();
	string data;
	while(!homework_list.empty())
	{
		query = string(" select homeworkstate from SendHomework where homeworkID=");
		query = query+homework_list.front()+" and classID="+Handle::int_to_string(classid)+";";
		cout<<query<<endl;
		if(mysql->select(query.c_str()))
		{
			cout<<"select view homeworkid error\n";
			homework_cpool.release_connection(mysql);
			return string();
		}

		if(!strcmp(mysql->res[0][0],"\0"))
		{
			mysql->free_result();
			query = string("select h.homeworkname,IFNULL(d.totalscore,0),d.rank  from (select studentID,@rownum := @rownum+1 as rank,homeworkID,totalscore from StudentHomework,(select @rownum:=0) r  where classID=");
			query = query+Handle::int_to_string(classid)+" and homeworkID="+homework_list.front()+" order by totalscore) d,Homework h where d.homeworkID=h.homeworkID and d.studentID="+Handle::int_to_string(studentid)+";";
			cout<<query<<endl;
			if(mysql->select(query.c_str()))
			{
				cout<<"select view homeworkid error\n";
				homework_cpool.release_connection(mysql);
				return string();
			}
			query = string();

			data = data+mysql->res[0][0]+","+mysql->res[0][1]+","+mysql->res[0][2];

			cout<<data<<endl;
		}
		else
			mysql->free_result();
		homework_list.pop_front();
		if(!homework_list.empty())
			data = data+":";
	}

	homework_cpool.release_connection(mysql);
	return data;
}




























