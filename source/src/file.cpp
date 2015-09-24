#include "file.h"
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
using namespace edwin;
using namespace std;
int File::row = 0;
File::~File()
{
	this->close();
}
void File::open(ios::openmode f_mode)
{
	fd.open(path.c_str(), f_mode);
}
bool File::write(string content)
{
	if(!fd.is_open())
	{
			cout<<"file not open\n";
			return false;
	}
	fd.write(content.c_str(), content.size());
	return fd.good();
}
int File::write_row(string content)
{
	if(!fd.is_open())
	{
			cout<<"file not open\n";
			return -1;
	}
	fd<<content<<endl;
	row++;
	return row;
}
string File::read()
{
	if(!fd.is_open())
	{
			cout<<"file not open\n";
			return string();
	}
	fd.seekg(0,fd.end);
	int lenght = fd.tellg();
	fd.seekg(0,fd.beg);
	char buff[lenght];
	fd.read(buff, lenght);
	return string(buff);
}
string File::read_row(int row)
{
	if(!fd.is_open())
	{
		cout<<"file not open\n";
		return string();
	}
	char buff[1500];
	for(int i = 0; i<row; i++)
	{
		fd.getline(buff, 1500);
	}
	return string(buff);

}
void File::close()
{
	fd.close();
}

