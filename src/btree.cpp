#include "btree.h"

#include <algorithm>

BTree::BTree(const std::string& file_name) :pager_(file_name, kDefaultPageSize) {
	
	// create header
	if (pager_.GetRealDBSize() == 0) {
		std::shared_ptr<MemPage> mem_page;
		
	}
	else {
		// check header 
		MemPage mem_page(0);
		pager_.ReadPage(mem_page);
		//stvector<>mem_page->getRowData();
		std::vector<uint8_t> raw_data = mem_page.getRawData();
		

	}
}

BTree::~BTree() {

}

void BTree::AllocateNode(PageNumber& page_number, PageType page_type) {
	pager_.AllocatePage(page_number);
}

/*
* Finds the data associated for a given key in a table B-Tree
*/
void BTree::Find(PageNumber root_page_number, KdbKey key) {	
	
	BTreeNode root(root_page_number);
	root.Read(pager_);

	PageNumber child_page_number;
	std::shared_ptr<BTreeCell> cell;
	root.Find(child_page_number, key);
	if (child_page_number == root_page_number) {
		// Found the target cell

	}
	else
		Find(child_page_number, key);
}

void BTree::InsertInIndex(PageNumber root_page_number, KdbKey keyIdx, KdbKey keyPk) {
	
	BTreeNode root(root_page_number);
	root.Read(pager_);

	IndexLeafCell cell(keyIdx, keyPk);
	root.InsertCellToSubtree(cell, pager_);

}

void BTree::InsertInTable(PageNumber root_page_number, KdbKey key,const std::vector<uint8_t>& data) {
	BTreeNode root(root_page_number);
	root.Read(pager_);

	TableLeafCell cell(key, data);
	root.InsertCellToSubtree(cell, pager_);
}

/*
*	
*/
BTreeNode::BTreeNode(PageNumber page_number, 
					PageType page_type,
					const std::vector<uint16_t>& cell_offset_array, 
					const std::vector<std::shared_ptr<BTreeCell>>& cells)
					: page_number_(page_number), page_type_(page_type),
					cell_offset_array_(cell_offset_array),cells_(cells),
					free_offset_(0), number_of_cells_(cells.size()),
					cells_offset_(kDefaultPageSize), right_page_(){

	for (auto cell : cells) {
		cells_offset_ -= cell->getSize();
	}

	free_offset_ += cells.size() * kCellOffsetSize;

	right_page_ = kDefaultRightPage;

}

BTreeNode::~BTreeNode(){

}


 bool BTreeNode::Find(std::shared_ptr<BTreeCell>, BTreeCell& cell) {

 }

 void  BTreeNode::InsertCellToSubtree(const BTreeCell& cell, Pager& pager) {

	std::shared_ptr<BTreeCell> cell;

	if (cells_offset_ - free_offset_ == 0) {
		//Split root
				
	}
 
	InsertToNonFullSubtree(cell, pager);
 }



 void BTreeNode::InsertToNonFullSubtree(const BTreeCell& cell, Pager& pager)
 {
	 // It is an internal node, determine what child node it must insert it in
	 if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal) {
		 
		 PageNumber child_page = -1;
		 for (auto iterator = cell_offset_array_.begin();iterator != cell_offset_array_.end();iterator++) {
			 if (cell.getKey() < cells_[*iterator]->getKey()) {
				 child_page = dynamic_cast<InternalCell&>(*cells_[*iterator]).getChildPage();
			 }
		 }

		 // child_page is the right page 
		 if (child_page == -1) {
			 child_page = right_page_;
		 }

		 /*
		 std::shared_ptr<MemPage> mem_page;
		 pager.ReadPage(mem_page, child_page);
		 std::shared_ptr<BTreeNode> child_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));
		 */

		 BTreeNode child_node(child_page);
		 child_node.Read(pager);

		 if (!child_node.CanInsertCell(cell))
			 Split(child_node, pager);


		 InsertToNonFullSubtree(cell, pager);
	 }
	 else {
		 // It is a leaf, this is where insertion actually happens 
		 InsertToNode(cell);
		 Write(pager);
	 }
 }

 /*
 *	
 */
 void BTreeNode::InsertToNode(const BTreeCell& cell) {
	
	 cells_.push_back(std::make_shared<BTreeCell>(cell));
	 cells_offset_ -= cell.getSize();
	 number_of_cells_++;

	 bool inserted = false;
	 for (auto iterator = cell_offset_array_.begin();iterator != cell_offset_array_.end();iterator++) {
		 if (cells_[*iterator]->getKey() > cell.getKey()) {
			 cell_offset_array_.insert(iterator, (uint16_t)cells_.size() - 1);
			 inserted = true;
		 }
	 }
	 if (!inserted)
		 cell_offset_array_.push_back((uint16_t)cells_.size() - 1);
	 free_offset_ += 2;
 }

 /*
 * Return 
 *  
 */
 bool BTreeNode::CanInsertCell(const BTreeCell& cell) {
	// 2 is space needs to store cell offset of this cell
	uint16_t space_need = cell.getSize() + 2;
	uint16_t space_remain = cells_offset_ - free_offset_;
	return space_remain >= space_need;
 } 

 /*
 *	Split the child_node into two nodes
 *	
 */
 void BTreeNode::Split(BTreeNode& child_node, Pager& pager) {
	 
	 std::vector<std::shared_ptr<BTreeCell>> cell_array_left, cell_array_right, cell_array = child_node.cells_;
	 std::vector<uint16_t> cell_offset_array_left, cell_offset_array_right, cell_offset_array = child_node.cell_offset_array_;
	 int medium_index = cell_offset_array.size() / 2;
	 
	 BTreeCell& medium_cell = *cell_array[cell_offset_array[medium_index]];

	 for (uint16_t i = 0;i < medium_index;i++) {
		cell_offset_array_left.push_back(i);
		cell_array_left.push_back(cell_array[cell_offset_array[i]]);
	 }

	 if (child_node.page_type_ == PageType::kIndexLeaf) {
		 cell_offset_array_left.push_back(medium_index);
		 cell_array_left.push_back(cell_array[cell_offset_array[medium_index]]);
	 }

	 for (uint16_t i = medium_index + 1;i < cell_offset_array_.size();i++) {
		 cell_offset_array_right.push_back(i - medium_index);
		 cell_array_right.push_back(cell_array[cell_offset_array[i]]);
	 }

	 // right_node is the previous child_node
	 BTreeNode right_node(child_node.page_number_, child_node.page_type_, cell_offset_array_right, cell_array_right);
	 right_node.Write(pager);

	 // Allocate a new page, prepare for a new btree node, build the allocated new node
	 PageNumber new_page_number;
	 pager.AllocatePage(new_page_number);
	 BTreeNode left_node(new_page_number, child_node.page_type_, cell_offset_array_left, cell_array_left);
	 if (child_node.page_type_ == PageType::kIndexInternal || child_node.page_type_ == PageType::kTableInternal)
		 left_node.right_page_ = dynamic_cast<InternalCell&>(medium_cell).getChildPage();
	 /*
	 if (child_node.page_type_ ==  PageType::kIndexInternal) {
		 left_node.right_page_ = dynamic_cast<IndexInternalCell&>(medium_cell).getChildPage();
	 }
	 else if (child_node.page_type_ == PageType::kTableInternal) {
		 left_node.right_page_ = dynamic_cast<TableInternalCell&>(medium_cell).getChildPage();
	 }
	 */
	 left_node.Write(pager);
	
	 // Make a cell based on the contents in medium_cell and insert it in current node(parent node)
	 std::shared_ptr<InternalCell> cell;
	 if (page_type_ == PageType::kIndexInternal)
	 {
		 KdbKey primary_key = dynamic_cast<IndexCell&>(medium_cell).getPrimaryKey();
		 /*
		 if (child_node.page_type_ == PageType::kIndexInternal) {
			 primary_key = dynamic_cast<IndexInternalCell&>(medium_cell).getPrimaryKey();
		 }
		 else if(child_node.page_type_ == PageType::kIndexLeaf) {
			 primary_key = dynamic_cast<IndexLeafCell&>(medium_cell).getPrimaryKey();
		 }
		 */
		 cell = std::make_shared<IndexInternalCell>(medium_cell.getKey(), primary_key, new_page_number);
	 }
	 else
		 cell = std::make_shared<TableInternalCell>(medium_cell.getKey(), new_page_number);
	 InsertToNode(*cell);
	 Write(pager);
 }

 void BTreeNode::Read(Pager& pager) {

	 MemPage mem_page(page_number_);
	 pager.ReadPage(mem_page);

	 std::vector<uint8_t> raw_data = mem_page.getRawData();

	 page_type_ = (PageType)raw_data[0];
	 free_offset_ = raw_data[1] << 8 + raw_data[2];
	 number_of_cells_ = raw_data[3] << 8 + raw_data[4];
	 cells_offset_ = raw_data[5] << 8 + raw_data[6];

	 // Only internal node has right page 
	 if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal)
		 right_page_ = raw_data[8] << 8 + raw_data[9] + raw_data[10] << 8 + raw_data[11];
	 else
		 right_page_ = kDefaultRightPage;

	 uint16_t index = 12;
	 uint16_t offset;
	 std::vector<uint16_t> offset_array;
	 while(index<free_offset_) {
		 offset = raw_data[index] + raw_data[index + 1];
		 index += 2;

		 offset_array.push_back(offset);
	 }

	 std::sort(offset_array.begin(), offset_array.end());
	 offset_array.push_back(kDefaultPageSize - 1);

	 for (int i = 1; i < offset_array.size();i++) {
		 std::shared_ptr<BTreeCell> cell = MakeCell(page_type_ ,raw_data, offset_array[i - 1], offset_array[i]);
		 
		 cells_.push_back(cell);
		 cell_offset_array_.push_back(i - 1);
	 }
 }


void BTreeNode::Write(Pager& pager) {

	std::vector<uint8_t> raw_data(kDefaultPageSize);
	
	raw_data[0] = (uint8_t)page_type_;
	raw_data[1] = free_offset_;
	raw_data[2] = free_offset_ >> 8;
	raw_data[3] = number_of_cells_;
	raw_data[4] = number_of_cells_ >> 8;
	raw_data[5] = cells_offset_;
	raw_data[6] = cells_offset_ >> 8;

	// Only internal node has right page 
	if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal)
		right_page_ = raw_data[8] << 8 + raw_data[9] + raw_data[10] << 8 + raw_data[11];
	else
		right_page_ = kDefaultRightPage;


	// Write offset array
	std::vector<uint16_t> cells_offset_cell_order(cell_offset_array_.size(),0);
	uint16_t cell_offset = kDefaultPageSize;
	for (auto cell : cells_) {
		cell_offset -= cell->getSize();
		cells_offset_cell_order.push_back(cell_offset);
	}

	std::vector<uint16_t> cells_offset(cell_offset_array_.size(), 0);
	for (uint16_t offset : cell_offset_array_) {
		cells_offset.push_back(cells_offset_cell_order[offset]);
	
	}

	uint16_t index = 12;
	for (uint16_t offset : cells_offset) {
		raw_data[index++] = offset;
		raw_data[index++] = offset >> 8;
	}

	// Write Cells
	index = kDefaultPageSize - 1;
	for (auto cell : cells_) {
		std::shared_ptr<std::vector<uint8_t>> cell_raw_data = cell->ConvertToRawData();
		
		for (int i = cell_raw_data->size() - 1;i >= 0;i--) {
			raw_data[index--] = (*cell_raw_data)[i];
		}
	}

	MemPage mem_page(page_number_);
	mem_page.setRawData(raw_data);
	pager.WritePage(mem_page);
}


std::shared_ptr<BTreeCell> BTreeNode::MakeCell(PageType page_type, const std::vector<uint8_t>& data, uint16_t start, uint16_t end) {
	KdbKey key;
	if (page_type == PageType::kTableInternal) {
		PageNumber child_page = data[start] | (data[start + 1] << 8) | (data[start + 2] << 16) | (data[start + 3] << 24);
		key = data[start + 4] | (data[start + 5] << 8) | (data[start + 6] << 16) | (data[start + 7] << 24);

		return std::make_shared<TableInternalCell>(key,child_page);
	}
	else if (page_type == PageType::kTableLeaf) {
		uint32_t record_size = data[start] | (data[start + 1] << 8) | (data[start + 2] << 16) | (data[start + 3] << 24);;
		key = data[start + 4] | (data[start + 5] << 8) | (data[start + 6] << 16) | (data[start + 7] << 24);
		
		std::vector<uint8_t> record_data(record_size);
		for (uint32_t i = 0; i < record_size;i++)
			record_data[i] = data[start + 8 + i];

		return std::make_shared<TableLeafCell>(key, data);
	}
	else if (page_type == PageType::kIndexInternal) {	
		PageNumber child_page = data[start] | (data[start + 1] << 8) | (data[start + 2] << 16) | (data[start + 3] << 24);
		key = data[start + 8] | (data[start + 9] << 8) | (data[start + 10] << 16) | (data[start + 11] << 24);
		KdbKey primary_key = data[start + 12] | (data[start + 13] << 8) | (data[start + 14] << 16) | (data[start + 15] << 24);
		return std::make_shared<IndexInternalCell>(key, primary_key, child_page);
	}
	else if (page_type == PageType::kIndexLeaf) {
		key = data[start + 4] | (data[start + 5] << 8) | (data[start + 6] << 16) | (data[start + 7] << 24);
		KdbKey primary_key = data[start + 8] | (data[start + 9] << 8) | (data[start + 10] << 16) | (data[start + 11] << 24);
		return std::make_shared<IndexLeafCell>(key, primary_key);
	}
}