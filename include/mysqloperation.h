#ifndef MYSQL_OPERATION_H
#define MYSQL_OPERATION_H
#include<mysql.h>

namespace edwin
{
	class MysqlOperation
	{
	public:
			MYSQL mysql;
			MYSQL *connection;
			MYSQL_RES *result;
			int line_num;
			char *** res;
			MysqlOperation();
			~MysqlOperation();	
			int connect(const char *account, const char *password, const char *database);
			int create_table(const char *query);
			int drop_table(const char *query);
			int select(const char* query);
			int insert(const char* query);
			int update(const char* query);
			int drop(const char *query);
			bool is_closed();
			void free_result();
			void close();
	};
}
#endif


