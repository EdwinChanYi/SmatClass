-- 创建数据库
create database IntelligenceClass character set utf8;

-- 选择数据库
use IntelligenceClass;

-- 增加 dbuser1 用户
--  创建用户‘edwin’密码为 ‘yw’拥有操作数据库edwin的所有权限
    GRANT ALL ON IntelligenceClass.* TO edwin IDENTIFIED BY "yw";
    flush privileges;

-- grant select,insert,update,delete on IntelligenceClass.* to edwin@localhost identified by "yw";
-- grant select,insert,update,delete on IntelligenceClass.* to edwin@'%' identified by "yw";

-- 创建表

-- 创建成员表
--DROP TABLE IF EXISTS 'Member';
                     
--成员表

create table Member
(
	memberID SMALLINT UNSIGNED NOT NULL,
	membername CHAR(20),
	sex BIT,
	password CHAR(20) NOT NULL,
	telephone CHAR(20),
	membertype TINYINT NOT NULL,
	
	CONSTRAINT memberkey PRIMARY KEY(memberID),
	CONSTRAINT memberunique UNIQUE(memberID, telephone)
)DEFAULT CHARSET=utf8;

-- 创建成员表
--DROP TABLE IF EXISTS 'Class';
                     
--班级表

create table Class
(
	classID SMALLINT UNSIGNED NOT NULL,
	classname CHAR(20),
	createtime DATETIME NOT NULL,
	classintrodution TEXT,
	classmanagerID SMALLINT,
	CONSTRAINT classkey PRIMARY KEY(classID),
	CONSTRAINT classunique UNIQUE(classID, classname)
)DEFAULT CHARSET=utf8;

-- 创建成员表
--DROP TABLE IF EXISTS 'ClassMember';
                     
--班级成员表

create table ClassMember
(
	memberID SMALLINT UNSIGNED NOT NULL,
	membername CHAR(20),
	classID SMALLINT UNSIGNED NOT NULL,
	classname CHAR(20),
	CONSTRAINT classmemberkey PRIMARY KEY(classID,memberID),
	CONSTRAINT memberforeign FOREIGN KEY (memberID)REFERENCES Member(memberID),
	CONSTRAINT classforeign FOREIGN KEY (classID)REFERENCES Class(classID)
)DEFAULT CHARSET=utf8;

-- 创建成员表
--DROP TABLE IF EXISTS 'TestModule';
                     
--测试模块表

create table TestModule
(
	moduleID SMALLINT UNSIGNED NOT NULL,
	modulename CHAR(20),
	coursename CHAR(20),
	teacherID SMALLINT UNSIGNED NOT NULL,
	modulepath VARCHAR(256),
	CONSTRAINT testmodulekey PRIMARY KEY(moduleID),
	CONSTRAINT testmoduleforeign FOREIGN KEY (teacherID)REFERENCES Member(memberID),
	CONSTRAINT testmodulecheck CHECK(teacherID>0 and teacherID<=100)
)DEFAULT CHARSET=utf8;

--测试题表
create table TestTitle
(
	titleID SMALLINT UNSIGNED NOT NULL,
	titlename CHAR(20),
	moduleID SMALLINT UNSIGNED NOT NULL,
	titlepath VARCHAR(256),
	rowID SMALLINT UNSIGNED,
	titleanswer CHAR(2),
	CONSTRAINT testtitlekey PRIMARY KEY(titleID, moduleID),
	CONSTRAINT testtitleforeign FOREIGN KEY(moduleID) REFERENCES TestModule(moduleID)
)DEFAULT CHARSET=utf8;
--DROP TABLE IF EXISTS 'StudentTest';
--学生测试表
create table StudentTest
(
	moduleID SMALLINT UNSIGNED NOT NULL,
	choisetitle VARCHAR(256),
	studentID SMALLINT UNSIGNED NOT NULL,
	titlepath VARCHAR(256),
	rowID SMALLINT UNSIGNED,
	testscore TINYINT UNSIGNED,
	CONSTRAINT studenttestkey PRIMARY KEY(studentID, moduleID),
	CONSTRAINT moduleforeign FOREIGN KEY(moduleID) REFERENCES TestModule(moduleID),
	CONSTRAINT studentforeign FOREIGN KEY(studentID) REFERENCES Member(memberID),
	CONSTRAINT studenttestcheck CHECK(studentID>100)
)DEFAULT CHARSET=utf8;
--作业基表
create table Homework
(
	homeworkID SMALLINT UNSIGNED NOT NULL,
	homeworkname CHAR(20),
	teacherID SMALLINT UNSIGNED NOT NULL,
	homeworkpath VARCHAR(256),
	homeworkanswer TEXT,
	CONSTRAINT homeworkkey PRIMARY KEY(homeworkID),
	CONSTRAINT homeworkforeign FOREIGN KEY(teacherID) REFERENCES Member(memberID),
	CONSTRAINT homeworkcheck CHECK(teacherID>0 and teacherID<=100)
)DEFAULT CHARSET=utf8;
--作业发布表
create table SendHomework
(
	homeworkID SMALLINT UNSIGNED NOT NULL,
	homeworkname CHAR(20) NOT NULL,
	sendtime INT,
	deadline INT,
	homeworkstate BIT,
	classID SMALLINT UNSIGNED NOT NULL,
	feedbacktable CHAR(30) NOT NULL,
	feedbackpath VARCHAR(256),
	CONSTRAINT sendhomeworkkey PRIMARY KEY(homeworkID, classID),
	CONSTRAINT sendhomeworkforeign FOREIGN KEY(homeworkID) REFERENCES Homework(homeworkID),
	CONSTRAINT sendclassforeign FOREIGN KEY(classID) REFERENCES Class(classID)
)DEFAULT CHARSET=utf8;
--学生作业表
create table StudentHomework
(
studentID SMALLINT UNSIGNED NOT NULL,
homeworkID SMALLINT UNSIGNED NOT NULL,
classID SMALLINT UNSIGNED NOT NULL,
checkstate BIT DEFAULT 0,
homeworkstate BIT DEFAULT 0,
choiceanswer CHAR(40),
subjectitemanswer TEXT,
subjectscore TINYINT UNSIGNED,
choicescore TINYINT UNSIGNED,
totalscore TINYINT UNSIGNED,
rank SMALLINT UNSIGNED,
CONSTRAINT studenthomeworkkey PRIMARY KEY(homeworkID, studentID),
CONSTRAINT studenthomeworkforeign FOREIGN KEY(homeworkID,classID) REFERENCES SendHomework(homeworkID,classID),
CONSTRAINT studentforeign FOREIGN KEY(studentID) REFERENCES Member(memberID)
)DEFAULT CHARSET=utf8;
--学生墙表
create table StudentWall
(
	wallID SMALLINT UNSIGNED NOT NULL,
	classID SMALLINT UNSIGNED NOT NULL,
	teacherID SMALLINT UNSIGNED NOT NULL,
	CONSTRAINT studentwallkey PRIMARY KEY(wallID),
	CONSTRAINT teacherforeign FOREIGN KEY(teacherID) REFERENCES Member(memberID),
	CONSTRAINT classforeign FOREIGN KEY(classID) REFERENCES Class(classID)
)DEFAULT CHARSET=utf8;
--评价基表
create table Evaluation
(
	evaluationID SMALLINT UNSIGNED NOT NULL,
	evaluationname CHAR(20),
	entertime INT NOT NULL,
	updatetime INT NOT NULL,
	evaluationpath VARCHAR(256),
	CONSTRAINT evaluationkey PRIMARY KEY(evaluationID)
)DEFAULT CHARSET=utf8;
--评价发布表
create table SendEvaluation
(
	evaluationID SMALLINT UNSIGNED NOT NULL,
	evaluationname CHAR(20) NOT NULL,
	sendtime INT NOT NULL,
	evaluationtype BIT,
	evaluationpath VARCHAR(256),
	CONSTRAINT sendevaluationkey PRIMARY KEY(evaluationID, evaluationname),
	CONSTRAINT evaluationIDforeign FOREIGN KEY(evaluationID) REFERENCES Evaluation(evaluationID)
)DEFAULT CHARSET=utf8;
--教师评价表
create table TeacherEvaluation
(
	evaluationID SMALLINT UNSIGNED NOT NULL,
	evaluationname CHAR(20) NOT NULL,
	sendstate BIT,
	sendtime DATETIME NOT NULL,
	deadline DATETIME NOT NULL,
	classID SMALLINT UNSIGNED NOT NULL,
	temptable CHAR(40),
	evaluationpath VARCHAR(256),
	feedbackpath VARCHAR(256),
	CONSTRAINT sendevaluationkey PRIMARY KEY(evaluationID, evaluationname, classID),
	CONSTRAINT sendevaluationIDforeign FOREIGN KEY(evaluationID) REFERENCES Evaluation(evaluationID),
	CONSTRAINT sendevaluationclassforeign FOREIGN KEY(classID) REFERENCES Class(classID)
)DEFAULT CHARSET=utf8;
commit;
