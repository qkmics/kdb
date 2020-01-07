#ifndef SQL_PARSER
#define SQL_PARSER

#include <string>
#include <memory>

#include "ksql.h"

class SqlParser {
public:
	static std::shared_ptr<KSqlStatement> Parse(const std::string& sql_string);



private:

};

#endif //  SQL_PARSER