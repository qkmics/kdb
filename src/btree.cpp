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


/*
*	Finds the data associated for a given key in a table B-Tree
*/

void BTree::Find(KdbKey key) {

}

void BTree::InsertInIndex(KdbKey keyIdx, KdbKey keyPk) {
	

}

void BTree::InsertInTable(KdbKey key, uint8_t* data, uint16_t size) {


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
 */m  
BTreeNode::~BTreeNode() {

}



void BTreeNode::InsertNonFull(CellNumber cell_number, BTreeCell& cell) {
	
	cells_.push_back(cell);
	cells_offset_.push_back();


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