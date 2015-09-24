/*
 * handle.h
 *
 *  Created on: Jul 24, 2015
 *      Author: miller
 */

#ifndef HANDLE_H_
#define HANDLE_H_
#include <string>
#include <list>
using namespace std;
namespace edwin
{
	class Handle
	{
	public:
		static std::string int_to_string(unsigned int value);
		static unsigned int string_to_int(std::string str);
		static string c_to_c(std::string &source, char change);
		static list<string> c_to_list(string &source,char change);

	};

}



#endif /* MYCODE_INCLUDE_HANDLE_H_ */
