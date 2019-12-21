#ifndef BTREE
#define BTREE

#include "pager.h"

#include <vector>
#include <string>
#include <memory>

enum class PageType {
	kTableInternal = 0x05,
	kTableLeaf = 0x0D,
	kIndexInternal = 0x02,
	kIndexLeaf = 0x0A
};

const int kDefaultPageSize = 1024;

class BTreeCell;
class BTree {
public:
	BTree(const std::string& file_name);
	~BTree();

	//void InsertCell(CellNumber cell_number, BTreeCell& cell);
	//void GetCell(CellNumber cell_number, BTreeCell& cell);

	void AllocateNode(PageNumber& page_number, PageType page_type);

	void Find(PageNumber root_number, KdbKey key);
	void InsertInIndex(PageNumber root_number, KdbKey keyIdx, KdbKey keyPk);
	void InsertInTable(PageNumber root_page_number, KdbKey key, const std::vector<uint8_t>& data);

private:
	//BTreeNode  root_;
	Pager pager_;
};

class BTreeNode {
public:
	BTreeNode(std::shared_ptr<MemPage> page);
	~BTreeNode();

	

	//void InsertCell(CellNumber cell_number, BTreeCell& cell);

	//void GetCell(CellNumber cell_number, BTreeCell& cell);

	void Write();

	bool Find(PageNumber& child_page, std::shared_ptr<BTreeCell>);

	void InsertCell(const BTreeCell& cell,Pager& pager);

private:
	bool CanInsertCell(const BTreeCell& cell);

	void InsertNonFull(const BTreeCell& cell, Pager& pager);

	void Split(BTreeNode& child_node, Pager& pager);


	// int chidb_Btree_split(BTree* bt, npage_t npage_parent, npage_t npage_child, ncell_t parent_ncell, npage_t* npage_child2)

	std::shared_ptr<MemPage> page_;             /* In-memory page returned by the Pager */
	PageType page_type_;              /* Type of page  */

	uint16_t free_offset_;      /* Byte offset of free space in page */
	CellNumber number_of_cells_;           /* Number of cells */
	uint16_t cells_offset_;     /* Byte offset of start of cells in page */
	PageNumber right_page_;        /* Right page (internal nodes only) */
	
	std::vector<uint16_t> cell_offset_array_; /* Pointer to start of cell offset array in the in-memory page */
	std::vector<std::shared_ptr<BTreeCell>> cells_;
};

class BTreeCell {
public:
	BTreeCell(KdbKey key) : key_(key) {}


	virtual uint16_t getSize() const= 0;

	KdbKey getKey() const{ return key_; }

private:
	KdbKey key_;
};

class InternalCell : public BTreeCell{
public:
	InternalCell(KdbKey key, PageNumber child_page) : BTreeCell(key), child_page_(child_page) {}

	PageNumber getChildPage() const { return child_page_; }

private:
	PageNumber child_page_;
};

class IndexInternalCell : public InternalCell {
public:
	IndexInternalCell(KdbKey key, KdbKey primary_key, PageNumber child_page) : InternalCell(key, child_page), primary_key_(primary_key){}

	uint16_t getSize() const { return 8; }

private:
	KdbKey primary_key_;
};


class TableInternalCell : public InternalCell {
public:
	TableInternalCell(KdbKey key, PageNumber child_page) : InternalCell(key, child_page){}

	uint16_t getSize() const { return 8; }

private:

};


class TableLeafCell : public BTreeCell {
public:
	TableLeafCell(KdbKey key, std::vector<uint8_t> data) : BTreeCell(key), data_(data) {}

	uint16_t getSize() const { return data_.size(); }

private:
	std::vector<uint8_t> data_;
};


class IndexLeafCell : public BTreeCell {
public:
	IndexLeafCell(KdbKey key, KdbKey primary_key) : BTreeCell(key), primary_key_(primary_key) {}

	uint16_t getSize() const { return 8; }

private:
	KdbKey primary_key_;
};


#endif // BTREE