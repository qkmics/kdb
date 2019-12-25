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
const PageNumber kDefaultRightPage = 0;
const uint16_t kCellOffsetSize = 2;


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

	BTreeNode(PageNumber page_number) : page_number_(page_number), right_page_(){}

	BTreeNode(PageNumber page_number, PageType page_type, const std::vector<uint16_t>& cell_offset_array, const std::vector<std::shared_ptr<BTreeCell>>& cells);
	
	~BTreeNode();

	void Read(Pager& pager);

	void Write(Pager& pager);

	bool Find(std::shared_ptr<BTreeCell>, BTreeCell& cell);

	void InsertCellToSubtree(const BTreeCell& cell,Pager& pager);

private:

	void InsertToNode(const BTreeCell& cell);

	bool CanInsertCell(const BTreeCell& cell);

	void InsertToNonFullSubtree(const BTreeCell& cell, Pager& pager);

	void Split(BTreeNode& child_node, Pager& pager);

	std::shared_ptr<BTreeCell> MakeCell(PageType page_type, const std::vector<uint8_t>& data, uint16_t start, uint16_t end);

	PageNumber page_number_;

	PageType page_type_;              /* Type of page  */

	std::vector<uint16_t> cell_offset_array_; /* Pointer to start of cell offset array in the in-memory page */

	std::vector<std::shared_ptr<BTreeCell>> cells_;


	uint16_t free_offset_;      /* Byte offset of free space in page */

	CellNumber number_of_cells_;           /* Number of cells */

	uint16_t cells_offset_;     /* Byte offset of start of cells in page */

	PageNumber right_page_;        /* Right page (internal nodes only) */
};



class BTreeCell {
public:
	BTreeCell(KdbKey key) : key_(key) {}

	virtual std::shared_ptr<std::vector<uint8_t>> ConvertToRawData() const = 0;
	
	virtual uint16_t getSize() const = 0;

	KdbKey getKey() const{ return key_; }

private:
	KdbKey key_;
};

class IndexCell : virtual public BTreeCell {
public:
	IndexCell(KdbKey key, KdbKey primary_key) : BTreeCell(key), primary_key_(primary_key) {}
	KdbKey getPrimaryKey() { return primary_key_; }
private:
	KdbKey primary_key_;

};

class InternalCell : virtual public BTreeCell {
public:
	InternalCell(KdbKey key, PageNumber child_page) : BTreeCell(key), child_page_(child_page) {}

	PageNumber getChildPage() const { return child_page_; }

private:
	PageNumber child_page_;
};

class IndexInternalCell : public InternalCell ,public IndexCell{
public:
	IndexInternalCell(KdbKey key, KdbKey primary_key, PageNumber child_page) : InternalCell(key, child_page), IndexCell(key, primary_key), BTreeCell(key) {}
	virtual std::shared_ptr<std::vector<uint8_t>> ConvertToRawData() const {}
	uint16_t getSize() const { return 8; }

private:

};

class IndexLeafCell : public IndexCell {
public:
	IndexLeafCell(KdbKey key, KdbKey primary_key) : IndexCell(key, primary_key), BTreeCell(key){}

	virtual std::shared_ptr<std::vector<uint8_t>> ConvertToRawData() const {}
	uint16_t getSize() const { return 8; }

private:
};

class TableInternalCell : public InternalCell {
public:
	TableInternalCell(KdbKey key, PageNumber child_page) : InternalCell(key, child_page){}

	virtual std::shared_ptr<std::vector<uint8_t>> ConvertToRawData() const  {}
	uint16_t getSize() const { return 8; }

private:

};

class TableLeafCell : public BTreeCell {
public:
	TableLeafCell(KdbKey key, std::vector<uint8_t> data) : BTreeCell(key), data_(data) {}
	virtual std::shared_ptr<std::vector<uint8_t>> ConvertToRawData() const  {}
	uint16_t getSize() const { return data_.size(); }

private:
	std::vector<uint8_t> data_;
};


#endif // BTREE