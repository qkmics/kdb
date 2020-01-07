#ifndef CODEGEN
#define CODEGEN

#include "dbm.h"
#include "../../include/ksql/ksql.h"

class CodeGenerator {
public:

	std::shared_ptr<DbMachine> GenerateStatement(const KSqlStatement& sql_statment);

private:
	std::shared_ptr<DbMachine> GenerateCreateStmt(const CreateStmt& sql_statment);
	std::shared_ptr<DbMachine> GenerateInsertStmt(const InsertStmt& sql_statment);
	std::shared_ptr<DbMachine> GenerateSelectStmt(const SelectStmt& sql_statment);


};
#endif  // CODEGEN