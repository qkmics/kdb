#ifndef SCHEMA
#define SCHEMA

#include <string>
#include <unordered_map>
#include "btree.h"
#include "../../include/ksql/ksql.h"

struct SchemaItem {
	std::string type_;						// "table" or "index"
	std::string indexed_table_name_;		// Associated table name
	int root_page_;							// Database page where the root node of the B-Tree is stored
	std::string sql_statement_;				// The SQL statement used to create the table or index.
	CreateStmt parsed_statement_;				// The parsed SQL statement
};	


class Schema {
public:

	// Load schema from the input btree
	void Load(const BTree& btree);

	// Load schema from the subtree rooted at root_page
	void LoadFromSubtree(const BTree& btree, int root_page);

	// Given a table name, determine whether such a table exists. 
	bool IsTableExist(const std::string& table_name) const;

	// Given a table name, obtain its root page.
	int GetRootPage(const std::string& table_name) const;

	// Given a table name and a column name, determine whether such a column exists in the table.
	bool IsColumnExist(const std::string& table_name, const std::string& column_name) const;

	// Given a table name and a column name, obtain the type of the column.
	bool GetColumnType(const std::string& table_name, const std::string& column_name) const;


private:

	// Keyed by schema item name
	std::unordered_map<std::string, SchemaItem> schema_table_;



	// Load schema from one cell
	void LoadOneCell(const BTreeCell& cell);

	
};

#endif //  SCHEMA