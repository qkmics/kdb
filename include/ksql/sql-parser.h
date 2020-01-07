#ifndef SQL_PARSER
#define SQL_PARSER

#include <string>
#include "ksql.h"
class SqlParser {
public:
	static KSqlStatement Parse(const std::string& sql_string);



private:

};

#endif //  SQL_PARSER