
#include "codegen.h"



std::shared_ptr<DbMachine> CodeGenerator::GenerateStatement(const KSqlStatement& sql_statment) {





}

std::shared_ptr<DbMachine> CodeGenerator::GenerateCreateStmt(const CreateStmt& sql_statment) {
	std::vector<Instruction> instructions;

	// Open the schema table using cursor 0
	instructions.push_back(Instruction(InstructionType::kInteger, 1, 0, -1, ""));
	instructions.push_back(Instruction(InstructionType::kOpenWrite, 0, 0, 5, ""));

	// Create a new B - Tree, store its root page in register 4
	instructions.push_back(Instruction(InstructionType::kCreateTable, 4, -1, -1, ""));

	// Create a new B - Tree, store its root page in register 4
	instructions.push_back(Instruction(InstructionType::kString, 5, 1, -1, "table"));
	instructions.push_back(Instruction(InstructionType::kString, 8, 2, -1, "products"));
	instructions.push_back(Instruction(InstructionType::kString, 8, 3, -1, "products"));
	instructions.push_back(Instruction(InstructionType::kString, 73, 5, -1, "CREATE TABLE products(code INTEGER PRIMARY KEY, name TEXT, price INTEGER)"));
	
	instructions.push_back(Instruction(InstructionType::kMakeRecord, 1, 5, 6, ""));
	instructions.push_back(Instruction(InstructionType::kInteger, 1, 7, -1, ""));
		 
	// Insert the new record
	instructions.push_back(Instruction(InstructionType::kInsert, 1, 0, -1, ""));
	
	// Close the cursor
	instructions.push_back(Instruction(InstructionType::kClose, 1, 0, -1, ""));

	return std::make_shared<DbMachine>(instructions);
}
std::shared_ptr<DbMachine> GenerateInsertStmt(const InsertStmt& sql_statment) {

}
std::shared_ptr<DbMachine> GenerateSelectStmt(const SelectStmt& sql_statment) {

}


