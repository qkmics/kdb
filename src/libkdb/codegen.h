#ifndef CODEGEN
#define CODEGEN

#include "dbm.h"
#include "../../include/ksql/ksql.h"

class CodeGenerator {
public:

	std::shared_ptr<DbMachine> GenerateDbm(const KSqlStatement& sql_statment);

private:

	std::shared_ptr<DbMachine> GenerateCreateTable();
	std::shared_ptr<DbMachine> GenerateInsert();
	std::shared_ptr<DbMachine> GenerateSelect();


};
#endif  // CODEGEN