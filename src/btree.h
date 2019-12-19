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

	void Find(KdbKey key);
	void InsertInIndex(KdbKey keyIdx, KdbKey keyPk);
	void InsertInTable(KdbKey key, uint8_t* data, uint16_t size);

private:
	//BTreeNode  root_;
	Pager pager_;
};

class BTreeNode {
public:
	BTreeNode(std::shared_ptr<MemPage> page);
	~BTreeNode();

	void InsertCell(CellNumber cell_number, BTreeCell& cell);
	

	void GetCell(CellNumber cell_number, BTreeCell& cell);

	void Write();

private:
	
	void InsertNonFull(CellNumber cell_number, BTreeCell& cell);

	std::shared_ptr<MemPage> page_;             /* In-memory page returned by the Pager */
	PageType page_type_;              /* Type of page  */

	uint16_t free_offset_;      /* Byte offset of free space in page */
	CellNumber number_of_cells_;           /* Number of cells */
	uint16_t cells_offset_;     /* Byte offset of start of cells in page */
	PageNumber right_page_;        /* Right page (internal nodes only) */
	
	std::vector<uint8_t> cell_offset_array_; /* Pointer to start of cell offset array in the in-memory page */
	std::vector<BTreeCell> cells_;
};

class BTreeCell {
public:
	virtual int getName() = 0;

	KdbKey getKey() { return key_; }

private:
	KdbKey key_;
};


class TableInternalCell : public BTreeCell {
public:


private:
	PageNumber child_page_;

};

class TableLeafCell : public BTreeCell {
public:


private:
	uint32_t data_size_;
	std::shared_ptr<uint8_t> data_;
};
class IndexInternalCell : public BTreeCell {
public:


private:
	KdbKey primary_key_;
	PageNumber child_page_;

};
class IndexLeafCell : public BTreeCell {
public:


private:
	KdbKey primary_key_;
};


#endif // BTREE