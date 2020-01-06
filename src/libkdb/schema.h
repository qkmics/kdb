#ifndef SCHEMA
#define SCHEMA

#include <string>
#include "btree.h"

struct TableInfo {
	int root_page;
	std::string sql_statement;
};

struct IndexInfo {
	std::string indexed_table_name;
	int root_page;
	std::string sql_statement;
};

class Schema {
public:

	// Load schema from the input btree
	void Load(const BTree& btree);


private:
	// Load schema from one cell
	void LoadOneCell(const BTreeCell& cell);

	// Given a table name, determine whether such a table exists. 
	bool IsTableExist(const std::string& table_name);

	// Given a table name, obtain its root page.
	int GetRootPage(const std::string& table_name);

	// Given a table name and a column name, determine whether such a column exists in the table.
	bool IsColumnExist(const std::string& table_name, const std::string& column_name);

	// Given a table name and a column name, obtain the type of the column.
	bool GetColumnType(const std::string& table_name, const std::string& column_name);
};

#endif //  SCHEMA