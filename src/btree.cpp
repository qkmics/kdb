#include "btree.h"

/* Open a B-Tree file
 *
 * This function opens a database file and verifies that the file
 * header is correct. If the file is empty (which will happen
 * if the pager is given a filename for a file that does not exist)
 * then this function will (1) initialize the file header using
 * the default page size and (2) create an empty table leaf node
 * in page 1.
 *
 * Parameters
 * - filename: Database file (might not exist)
 * - db: A chidb struct. Its bt field must be set to the newly
 *       created BTree.
 * - bt: An out parameter. Used to return a pointer to the
 *       newly created BTree.
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ECORRUPTHEADER: Database file contains an invalid header
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
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

/* Create a new B-Tree node
 *
 * Allocates a new page in the file and initializes it as a B-Tree node.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage: Out parameter. Returns the number of the page that
 *          was allocated.
 * - type: Type of B-Tree node (PGTYPE_TABLE_INTERNAL, PGTYPE_TABLE_LEAF,
 *         PGTYPE_INDEX_INTERNAL, or PGTYPE_INDEX_LEAF)
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */

void BTree::AllocateNode(PageNumber& page_number, PageType page_type) {
	pager_.AllocatePage(page_number);


}


/* Find an entry in a table B-Tree
 *
 * Finds the data associated for a given key in a table B-Tree
 *
 * Parameters
 * - bt: B-Tree file
 * - nroot: Page number of the root node of the B-Tree we want search in
 * - key: Entry key
 * - data: Out-parameter where a copy of the data must be stored
 * - size: Out-parameter where the number of bytes of data must be stored
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_ENOTFOUND: No entry with the given key way found
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
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
	btree_node->InsertCell(cell, pager_);
}

void BTree::InsertInTable(PageNumber root_page_number, KdbKey key,const std::vector<uint8_t>& data) {
	std::shared_ptr<MemPage> mem_page;
	pager_.ReadPage(mem_page, root_page_number);
	std::shared_ptr<BTreeNode> btree_node = std::make_shared<BTreeNode>(BTreeNode(mem_page));
	
	TableLeafCell cell(key, data);
	btree_node->InsertCell(cell, pager_);
}

/* Loads a B-Tree node from disk
 *
 * Reads a B-Tree node from a page in the disk. All the information regarding
 * the node is stored in a BTreeNode struct (see header file for more details
 * on this struct). *This is the only function that can allocate memory for
 * a BTreeNode struct*. Always use chidb_Btree_freeMemNode to free the memory
 * allocated for a BTreeNode (do not use free() directly on a BTreeNode variable)
 * Any changes made to a BTreeNode variable will not be effective in the database
 * until chidb_Btree_writeNode is called on that BTreeNode.
 *
 * Parameters
 * - bt: B-Tree file
 * - npage: Page of node to load
 * - btn: Out parameter. Used to return a pointer to newly creater BTreeNode
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EPAGENO: The provided page number is not valid
 * - CHIDB_ENOMEM: Could not allocate memory
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
BTreeNode::BTreeNode(std::shared_ptr<MemPage> page) :page_(page) {

	PageType page_type_;              /* Type of page  */

	uint16_t free_offset_;      /* Byte offset of free space in page */
	CellNumber number_of_cells_;           /* Number of cells */
	uint16_t cells_offset_;     /* Byte offset of start of cells in page */
	PageNumber right_page_;        /* Right page (internal nodes only) */

	std::vector<uint8_t> cell_offset_array_; /* Pointer to start of cell offset array in the in-memory page */
	std::vector<BTreeCell> cells_;

	 
	std::vector<uint8_t> raw_data = page_->getRawData();
	
	page_type_ = (PageType)raw_data[0];
	free_offset_ = raw_data[1] << 8 + raw_data[2];
	number_of_cells_= raw_data[3] << 8 + raw_data[4];
	cells_offset_= raw_data[5] << 8 + raw_data[6];

	// Node of internal type has right page 
	if (page_type_ == PageType::kIndexInternal || page_type_ == PageType::kTableInternal)
		right_page_ = raw_data[8] << 8 + raw_data[9] + raw_data[10] << 8 + raw_data[11];
	else
		right_page_ = 0;
}



/* Frees the memory allocated to an in-memory B-Tree node
 *
 * Frees the memory allocated to an in-memory B-Tree node, and
 * the in-memory page returned by the pages (stored in the
 * "page" field of BTreeNode)
 *
 * Parameters
 * - bt: B-Tree file
 * - btn: BTreeNode to free
 *
 * Return
 * - CHIDB_OK: Operation successful
 */
BTreeNode::~BTreeNode() {

}


 bool BTreeNode::Find(std::shared_ptr<BTreeCell>, BTreeCell& cell) {

 }

 void  BTreeNode::InsertCell(const BTreeCell& cell, Pager& pager) {

	std::shared_ptr<BTreeCell> cell;

	if (cells_offset_ - free_offset_ == 0) {
		//Split root
				
	}
 
	InsertNonFull(cell, pager);
 }



 /* Insert a BTreeCell into a non-full B-Tree node
  *
  * chidb_Btree_insertNonFull inserts a BTreeCell into a node that is
  * assumed not to be full (i.e., does not require splitting). If the
  * node is a leaf node, the cell is directly added in the appropriate
  * position according to its key. If the node is an internal node, the
  * function will determine what child node it must insert it in, and
  * calls itself recursively on that child node. However, before doing so
  * it will check if the child node is full or not. If it is, then it will
  * have to be split first.
  *
  * Parameters
  * - bt: B-Tree file
  * - nroot: Page number of the root node of the B-Tree we want to insert
  *          this cell in.
  * - btc: BTreeCell to insert into B-Tree
  *
  * Return
  * - CHIDB_OK: Operation successful
  * - CHIDB_EDUPLICATE: An entry with that key already exists
  * - CHIDB_ENOMEM: Could not allocate memory
  * - CHIDB_EIO: An I/O error has occurred when accessing the file
  */
 void BTreeNode::InsertNonFull(const BTreeCell& cell, Pager& pager)
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

		 child_node->InsertNonFull(cell, pager);
	 }
	 else {

		 cells_.push_back(std::make_shared<BTreeCell>(cell));
		 bool inserted = false;
		 for (auto iterator = cell_offset_array_.begin();iterator != cell_offset_array_.end();iterator++) {
			 if (cells_[*iterator]->getKey() > cell.getKey()) {
				 cell_offset_array_.insert(iterator, (uint16_t)cells_.size() - 1);
				 inserted = true;
			 }
		 }
		 if (!inserted)
			 cell_offset_array_.push_back((uint16_t)cells_.size() - 1);
	 }
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


 void BTreeNode::Split(BTreeNode& child_node, Pager& pager) {
	 
	 int medium_index = cell_offset_array_.size() / 2;
	 

	 
	 
	 std::vector<std::shared_ptr<BTreeCell>> cell_array_left;


	 for (int i = 0;i < medium_index;i++) {

	 }


 }

/* Write an in-memory B-Tree node to disk
 *
 * Writes an in-memory B-Tree node to disk. To do this, we need to update
 * the in-memory page according to the chidb page format. Since the cell
 * offset array and the cells themselves are modified directly on the
 * page, the only thing to do is to store the values of "type",
 * "free_offset", "n_cells", "cells_offset" and "right_page" in the
 * in-memory page.
 *
 * Parameters
 * - bt: B-Tree file
 * - btn: BTreeNode to write to disk
 *
 * Return
 * - CHIDB_OK: Operation successful
 * - CHIDB_EIO: An I/O error has occurred when accessing the file
 */
void BTreeNode::Write() {

	std::vector<uint8_t> raw_data = page_->getRawData();
	
	if(page_type_== PageType::)


	

}