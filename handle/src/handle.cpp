/*
 * handle.cpp
 *
 *  Created on: Jul 24, 2015
 *      Author: miller
 */
#include <iostream>
#include <string>
#include "handle.h"
#include <cstdio>
#include <list>
#include <sstream>
using namespace edwin;
using namespace std;
string Handle::int_to_string(unsigned int value)
{
	char buff[10] = {0};
	sprintf(buff, "%d", value);
	return string(buff);
}
string Handle::c_to_c(string &source,char change)
{
	int i=0;
	list<string> diretion_value;
	while(source[i])
	{
		if(source[i] == change)
			source[i] = ' ';
		i++;
	}
	return source;
}
list<string> Handle::c_to_list(string &source, char change)
{
	int i=0;
	list<string> diretion_value;
	while(source[i])
	{
		if(source[i] == change)
			source[i] = ' ';
		i++;
	}
	istringstream change_per(source);
	string temp;
	int legth = source.size();
	while(legth>0)
	{
		change_per>>temp;
		legth = legth-temp.size()-1;
		diretion_value.push_back(temp);
	}
	return diretion_value;
}






