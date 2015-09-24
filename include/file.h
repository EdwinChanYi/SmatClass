#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <fstream>
#include <string>
#include <ios>

using namespace std;

namespace edwin
{
	class File
	{
		private:
			string path;
			ios_base::openmode mode;
			fstream fd;
			static int row;
		public:			
			File(string &f_path, ios_base::openmode f_mode):path(f_path),mode(f_mode){}
			virtual ~File();
			void open(ios::openmode f_mode);
			bool write(string content);
			int write_row(string content);
			string read();
			string read_row(int row);
			void close();
	};
}
#endif
