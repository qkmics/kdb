#include "btree.h"

BTree::BTree(const std::string& file_name) :pager_(file_name, kDefaultPageSize) {
	
	// create header
	if (pager_.GetRealDBSize() == 0) {
		std::shared_ptr<MemPage> mem_page;
		
	}
	else {
		// check header 

		std::shared_ptr<MemPage> mem_page;
		pager_.ReadPage(mem_page, 0);
		//stvector<>mem_page->getRowData();
		std::vector<uint8_t> raw_data = mem_page->getRawData();
		

	}
}

BTree::~BTree() {

}

void BTree::AllocateNode(PageNumber& page_number, PageType page_type) {
	pager_.AllocatePage(page_number);


}


void BTree::Find(PageNumber root_page_number, KdbKey key) {	
	std::shared_ptr<MemPage> mem_page;

	pager_.ReadPage(mem_page, root_page_number);
	std::shared_ptr<BTreeNode> btree_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));

	PageNumber child_page_number;
	std::shared_ptr<BTreeCell> cell;
	btree_node->Find(child_page_number,cell);
	if (child_page_number == root_page_number) {
		// Found the target cell

	}
	else
		Find(child_page_number, key);
}

void BTree::InsertInIndex(PageNumber root_page_number, KdbKey keyIdx, KdbKey keyPk) {
	
	std::shared_ptr<MemPage> mem_page;
	pager_.ReadPage(mem_page, root_page_number);
	std::shared_ptr<BTreeNode> btree_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));
	
	IndexLeafCell cell(keyIdx, keyPk);
	btree_node->InsertCellToSubtree(cell, pager_);
}

void BTree::InsertInTable(PageNumber root_page_number, KdbKey key,const std::vector<uint8_t>& data) {
	std::shared_ptr<MemPage> mem_page;
	pager_.ReadPage(mem_page, root_page_number);
	std::shared_ptr<BTreeNode> btree_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));
	
	TableLeafCell cell(key, data);
	btree_node->InsertCellToSubtree(cell, pager_);
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

	free_offset_ += cells.size() * 2;

	PageNumber right_page_;        /* Right page (internal nodes only) */

}

BTreeNode::~BTreeNode(){
	PageType page_type_;              /* Type of page  */

	uint16_t free_offset_;      /* Byte offset of free space in page */
	CellNumber number_of_cells_;           /* Number of cells */
	uint16_t cells_offset_;     /* Byte offset of start of cells in page */
	PageNumber right_page_;        /* Right page (internal nodes only) */

	std::vector<uint8_t> cell_offset_array_; /* Pointer to start of cell offset array in the in-memory page */
	std::vector<BTreeCell> cells_;
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

		 std::shared_ptr<MemPage> mem_page;
		 pager.ReadPage(mem_page, child_page);
		 std::shared_ptr<BTreeNode> child_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));

		 if (!child_node->CanInsertCell(cell))
			 Split(*child_node, pager);

		 child_node->InsertToNonFullSubtree(cell, pager);
	 }
	 else {
		 // It is a leaf, this is where insertion actually happens 
		 InsertToNode(cell);

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
 void BTreeNode::Split(BTreeNode&& child_node, Pager& pager) {
	 
	 // Allocate a new page, prepare for a new btree node 
	 PageNumber new_page_number;
	 pager.AllocatePage(new_page_number);

	 int medium_index = cell_offset_array_.size() / 2;
	 
	 std::vector<std::shared_ptr<BTreeCell>> cell_array_left, cell_array_right;
	 std::vector<uint16_t> cell_offset_array_left, cell_offset_array_right;

	 for (uint16_t i = 0;i < medium_index;i++) {
		cell_offset_array_left.push_back(i);
		cell_array_left.push_back(cells_[cell_offset_array_[i]]);
	 }

	 if (page_type_ == PageType::kIndexLeaf) {
		 cell_offset_array_left.push_back(medium_index);
		 cell_array_left.push_back(cells_[cell_offset_array_[medium_index]]);
	 }

	 for (uint16_t i = medium_index + 1;i < cell_offset_array_.size();i++) {
		 cell_offset_array_right.push_back(i - medium_index);
		 cell_array_right.push_back(cells_[cell_offset_array_[i]]);
	 }


	 BTreeNode left_node(page_number_,page_type_, cell_offset_array_left, cell_array_left);
	 BTreeNode right_node(new_page_number, page_type_, cell_offset_array_right, cell_array_right);

	
	 std::shared_ptr<BTreeCell> medium_cell = cells_[cell_offset_array_[medium_index]];

	 std::shared_ptr<InternalCell> cell;
	 if (page_type_ == PageType::kIndexInternal)
		 cell = std::make_shared<IndexInternalCell>(medium_cell);       ////////////////////////////// Problem
	 else
		 cell = std::make_shared<TableInternalCell>(medium_cell->getKey(), new_page_number);

	 InsertToNode(*cell);

	

	 child_node = left_node;		//update child_node

	 MemPage mem_page(new_page_number);
	 right_node.Write(mem_page);
	 pager.WritePage(mem_page);
 }

 void BTreeNode::Read(const MemPage& page) {

	 std::vector<uint8_t> raw_data = page.getRawData();

	 page_type_ = (PageType)raw_data[0];
	 free_offset_ = raw_data[1] << 8 + raw_data[2];
	 number_of_cells_ = raw_data[3] << 8 + raw_data[4];
	 cells_offset_ = raw_data[5] << 8 + raw_data[6];

	 // Only internal node has right page 
	 if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal)
		 right_page_ = raw_data[8] << 8 + raw_data[9] + raw_data[10] << 8 + raw_data[11];
	 else
		 right_page_ = 0;
 }

void BTreeNode::Write(MemPage& page) {

	std::vector<uint8_t> raw_data = page.getRawData();
	
	page_type_ = (PageType)raw_data[0];
	free_offset_ = raw_data[1] << 8 + raw_data[2];
	number_of_cells_ = raw_data[3] << 8 + raw_data[4];
	cells_offset_ = raw_data[5] << 8 + raw_data[6];

	// Only internal node has right page 
	if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal)
		right_page_ = raw_data[8] << 8 + raw_data[9] + raw_data[10] << 8 + raw_data[11];
	else
		right_page_ = 0;


	page.setRawData(raw_data);
	
}