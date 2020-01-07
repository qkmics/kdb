#include "schema.h"
#include "record.h"
#include "../../include/ksql/sql-parser.h"

// Load schema from the input btree
void Schema::Load(const BTree& btree) {
	LoadFromSubtree(btree, 1); // Page 1 is the root page of schema table
}

void Schema::LoadFromSubtree(const BTree& btree ,int root_page){
	BTreeNode node(root_page);
	const std::vector<std::shared_ptr<BTreeCell>> cells = node.GetCells();
	if (node.getType() == PageType::kTableInternal) {
		for (std::shared_ptr<BTreeCell> cell : cells) {
			TableInternalCell& table_internal_cell = **(std::static_pointer_cast<std::shared_ptr<TableInternalCell>>(cell));
			PageNumber child_page = table_internal_cell.getChildPage();
			LoadFromSubtree(btree, child_page);
		}
		if (node.ExistRightPage) {
			LoadFromSubtree(btree, node.getRightPage());
		}
	}
	else if (node.getType() == PageType::kTableLeaf) {
		for (std::shared_ptr<BTreeCell> cell : cells) {
			LoadOneCell(*cell);
		}
	}
	else {
		// error;
	}
}

// Given a table name, determine whether such a table exists. 
bool Schema::IsTableExist(const std::string& table_name) const {
	return schema_table_.find(table_name) != schema_table_.end();
}

// Given a table name, obtain its root page.
int Schema::GetRootPage(const std::string& table_name) const {
	//return schema_table_[table_name];
	return schema_table_.find(table_name)->second.root_page_;
}

// Given a table name and a column name, determine whether such a column exists in the table.
bool Schema::IsColumnExist(const std::string& table_name, const std::string& column_name) const {

}

// Given a table name and a column name, obtain the type of the column.
bool Schema::GetColumnType(const std::string& table_name, const std::string& column_name) const {

}

// Load schema from one cell
void Schema::LoadOneCell(const BTreeCell& cell) {
	const TableLeafCell& schema_item_cell = static_cast<const TableLeafCell&>(cell);
	KdbRecord record(*schema_item_cell.ConvertToRawData());
	SchemaItem item;
	item.type_ = record.getString(0);
	std::string item_name = record.getString(1);
	item.indexed_table_name_ = record.getString(2);
	item.root_page_ = record.getInteger(3);
	item.sql_statement_ = record.getString(4);
	std::shared_ptr<CreateStmt> creat_stmt=(std::static_pointer_cast<CreateStmt>(SqlParser::Parse(item.sql_statement_)));
	item.parsed_statement_ = *creat_stmt;
	schema_table_[item_name] = item;
}