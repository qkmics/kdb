#ifndef CODEGEN
#define CODEGEN

#include "dbm.h"
#include "../include/ksql/ksql.h"

class CodeGenerator {
public:

	std::shared_ptr<DbMachine> GenerateDbm(const KSqlStatement& sql_statment);




private:


};
#endif  // CODEGEN