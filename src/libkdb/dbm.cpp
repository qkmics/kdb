
#include "dbm.h"
#include "record.h"
#include <iostream>

int Register::Compare(const Register& reg) const {
	
	int compare_result = 0;

	if (type_ != reg.type_) {
		std::cout << "Register::Compare error" << std::endl;
	}


	if (type_ == RegisterType::kInt32) {
		if (integer_ > reg.integer_)
			compare_result = 1;
		else if(integer_ < reg.integer_)
			compare_result = -1;
	}
	else if (type_ == RegisterType::kString) {
		if (string_ > reg.string_)
			compare_result = 1;
		else if (string_ < reg.string_)
			compare_result = -1;
	}
	else if (type_ == RegisterType::kBinary) {

	}
	else {
		std::cout << "Register::Compare error" << std::endl;
	}
	return compare_result;
}

/* Run the DBM
 *
 * This function will run the DBM until one of the following happens:
 *
 *  - A ResultRow instruction is encountered. startRR and nRR are set
 *    to the appropriate values, and the function returns CHIDB_ROW

 *  - A Halt instruction is encountered, and CHIDB_DONE is returned.
 
 *  - The end of the program is reached (the program counter goes
 *    beyond endOp), and CHIDB_DONE is returned.
 
 *  - Any of the instructions returns anything other than CHIDB_OK
 *    or CHIDB_ROW. The program stops executing and and the return
 *    value of the instruction handler is returned.
 *
 * Parameters
 * - stmt: DBM to run.
 *
 * Returns
 * - CHIDB_ROW: Statement returned a row.
 * - CHIDB_DONE: Statement has finished executing.
 * - Any error code returned by an individual instruction handler.
 */
void DbMachine::Run() {
	while (pc_ < instructions_.size()) {
		instruction_ = instructions_[pc_];
		switch (instruction_.getType()) {
		case InstructionType::kInteger:
			Integer();
			break;
		case InstructionType::kString:
			String();
			break;
		case InstructionType::kNull:
			Null();
			break;
		case InstructionType::kSCopy:
			SCopy();
			break;
		case InstructionType::kEq:
			Eq();
			break;
		case InstructionType::kNe:
			Ne();
			break;
		case InstructionType::kLt:
			Lt();
			break;
		case InstructionType::kLe:
			Le();
			break;
		case InstructionType::kGt:
			Gt();
			break;
		case InstructionType::kGe:
			Ge();
			break;
		case InstructionType::kHalt:
			Halt();
			break;
		case InstructionType::kNoop:
			break;
		case InstructionType::kOpenRead:
			OpenRead();
			break;
		case InstructionType::kOpenWrite:
			OpenWrite();
			break;
		case InstructionType::kClose:
			Close();
			break;
		case InstructionType::kRewind:
			Rewind();
			break;
		case InstructionType::kNext:
			Next();
		case InstructionType::kPrev:
			Prev();
			break;
		case InstructionType::kSeek:
			Seek();
			break;
		case InstructionType::kSeekGt:
			SeekGt();
			break;
		case InstructionType::kSeekGe:
			SeekGe();
			break;
		case InstructionType::kSeekLt:
			SeekLt();
			break;
		case InstructionType::kSeekLe:
			SeekLe();
			break;
		case InstructionType::kIdxGt:
			IdxGt();
			break;
		case InstructionType::kIdxGe:
			IdxGe();
			break;
		case InstructionType::kIdxLt:
			IdxLt();
			break;
		case InstructionType::kIdxLe:
			IdxLe();
			break;
		case InstructionType::kColumn:
			Column();
			break;
		case InstructionType::kKey:
			Key();
			break;
		case InstructionType::kIdxPKey:
			IdxPKey();
			break;
		case InstructionType::kMakeRecord:
			MakeRecord();
			break;
		case InstructionType::kResultRow:
			ResultRow();
			break;
		case InstructionType::kInsert:
			Insert();
			break;
		case InstructionType::kIdxInsert:
			IdxInsert();
			break;
		case InstructionType::kCreateTable:
			CreateTable();
			break;
		case InstructionType::kCreateIndex:
			CreateIndex();
			break;
		default:
			std::cout << "error in switch" << std::endl;
		}

	}


}

void DbMachine::Integer() {
	Instruction& instruction = instructions_[pc_];
	int integer = instruction.getP1();
	int register_number = instruction.getP2();
	registers_[register_number]->setData(integer);
}

void DbMachine::String() {
	Instruction& instruction = instructions_[pc_];
	int length = instruction.getP1();
	int register_number = instruction.getP2();
	std::string string_data = instruction.getP4();
	registers_[register_number]->setData(string_data);

}

void DbMachine::Null() {
	Instruction& instruction = instructions_[pc_];
	int register_number = instruction.getP2();
	registers_[register_number]->setData();

}

// Make a shallow copy of the contents of r1 into r2. 
// i.e., r2 must be left pointing to the same value as r1.
void DbMachine::SCopy() {
	int register_number_from = instruction_.getP2();
	int register_number_to = instruction_.getP2();
	*registers_[register_number_to] = *registers_[register_number_from];
}

// If the contents of r1 are equal to the contents of r2, jump to j. 
// Otherwise, do nothing. This instruction assumes that the types of the contents of 
// both registers are the same. The behaviour when either of the registers is NULL is undefined.
void DbMachine::Eq() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) == 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Ne() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) != 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Lt() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) < 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Le() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) <= 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Gt() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) > 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Ge() {
	int register_number1 = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number2 = instruction_.getP3();
	if (registers_[register_number1]->Compare(*registers_[register_number2]) >= 0) {
		pc_ = jump_address;
	}
}
void DbMachine::Halt() {
	int register_number = instruction_.getP1();
	std::string error_message = instruction_.getP4();
}

// Opens the B-Tree rooted at the page n for read-only access and stores a cursor for it in c
void DbMachine::OpenRead() {
	int cursor_number = instruction_.getP1();
	int register_number = instruction_.getP2();
	int page_number = registers_[register_number]->getInteger();
	int number_of_columns = instruction_.getP3();   // The number of columns in the table (0 if opening an index)
	cursors_[cursor_number].OpenRead(page_number);
}

// Same as OpenRead but opening the B-Tree in read/write mode
void DbMachine::OpenWrite() {
	int cursor_number = instruction_.getP1();
	int register_number = instruction_.getP2();
	int page_number = registers_[register_number]->getInteger();
	int number_of_columns = instruction_.getP3();
	cursors_[cursor_number].OpenRead(page_number);
}

// Closes cursor c and frees up any resources associated with it.
void DbMachine::Close() {
	int cursor_number = instruction_.getP1();
	cursors_[cursor_number].Close();
}

// Makes cursor c point to the first entry in the B-Tree. 
// If the B-Tree is empty, then jump to j.
void DbMachine::Rewind() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	if (!cursors_[cursor_number].ReWind()) {
		pc_ = jump_address - 1;
	}
}

// Advance cursor c to the next entry in the B-Tree and jump to j. 
// If there are no more entries (if cursor c was pointing at the last entry in the B-Tree), do nothing.
void DbMachine::Next() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	if (cursors_[cursor_number].Next()) {
		pc_ = jump_address - 1;
	}
}

void DbMachine::Prev() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	if (cursors_[cursor_number].Prev()) {
		pc_ = jump_address - 1;
	}
}

// Move cursor c to point to the entry with key equal to k. 
// If the cursor points to an index B-Tree, the key that must match is 
// IdxKey in the (IdxKey,PKey) pair. If the B-Tree doesn’t contain k, jump to j.
void DbMachine::Seek() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	if (!cursors_[cursor_number].Seek(key)) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::SeekGt() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	if (!cursors_[cursor_number].SeekGt(key)) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::SeekGe() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	if (!cursors_[cursor_number].SeekGe(key)) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::SeekLt() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	if (!cursors_[cursor_number].SeekLt(key)) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::SeekLe() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	if (!cursors_[cursor_number].SeekLe(key)) {
		pc_ = jump_address - 1;
	}
}

// This instructions indicates that a result row has been produced and pauses execution 
// for the database machine user to fetch the result row. 
// The result row is formed by the values stored in registers r through r+n−1.
void DbMachine::ResultRow() {
	result_row_start_register_ = instruction_.getP1();
	result_row_size_ = instruction_.getP2();
}

// Store in register r the value in the n-th column of the entry pointed at by cursor c. 
// Columns are numbered from 0.
void DbMachine::Column() {
	int cursor_number = instruction_.getP1();
	int column_number = instruction_.getP2();
	int register_number = instruction_.getP3();
	
	registers_[register_number] = cursors_[cursor_number].GetColumn(column_number);
}

// Store in register r the value of the key of the entry pointed at by cursor c.
void DbMachine::Key() {
	int cursor_number = instruction_.getP1();
	int register_number = instruction_.getP2();
	registers_[register_number]->setData(cursors_[cursor_number].GetKey());
}

// Create a database record using the values from registers r1 through r1+n−1, and store the record in r2.
void DbMachine::MakeRecord() {
	int register_number_start = instruction_.getP1();
	int data_size = instruction_.getP2();
	int register_number_end = register_number_start + data_size - 1;
	int register_number_target = instruction_.getP3();
	
	KdbRecord record(registers_, register_number_start, register_number_end);
	std::vector<uint8_t> data = record.getRawData();
	registers_[register_number_target]->setData(data);
}

// Inserts an entry, with key k and value v,
// in the B-Tree pointed at by cursor c.
void DbMachine::Insert() {
	int cursor_number = instruction_.getP1();
	int register_number_record = instruction_.getP2(); // The register must contain a database record v
	int register_number_key = instruction_.getP3();	   // The register must contain a key k
	const std::vector<uint8_t>& record = registers_[register_number_record]->getBytes();
	int key = registers_[register_number_key]->getInteger();
	cursors_[cursor_number].Insert(key, record);
}

// CREATE TABLE statements
// Create a new table B-Tree and store its root page in r.
void DbMachine::CreateTable() {
	int register_number = instruction_.getP1();
	BTree table_btree(kdb_file_name_);
	registers_[register_number]->setData(table_btree.getRootPage());
}

// Cursor c points to an index entry containing a (IdxKey,PKey) pair. 
// If IdxKey is greater than k, jump to j. Otherwise, do nothing.
void DbMachine::IdxGt() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	int index_key = cursors_[cursor_number].GetIndexKey();
	if (index_key > key) {
		pc_ = jump_address - 1;
	}
}

void DbMachine::IdxGe() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	int index_key = cursors_[cursor_number].GetIndexKey();
	if (index_key >= key) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::IdxLt() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	int index_key = cursors_[cursor_number].GetIndexKey();
	if (index_key < key) {
		pc_ = jump_address - 1;
	}
}
void DbMachine::IdxLe() {
	int cursor_number = instruction_.getP1();
	int jump_address = instruction_.getP2();
	int register_number = instruction_.getP3();
	int key = registers_[register_number]->getInteger();
	int index_key = cursors_[cursor_number].GetIndexKey();
	if (index_key <= key) {
		pc_ = jump_address - 1;
	}
}

// Cursor c points to an index entry containing a (IdxKey,PKey) pair. Store PKey in r.
void DbMachine::IdxPKey() {
	int cursor_number = instruction_.getP1();
	int register_number = instruction_.getP2();

	registers_[register_number]->setData(cursors_[cursor_number].GetIdxPKey());
}

void DbMachine::IdxInsert() {
	int cursor_number = instruction_.getP1();
	int register_number_index_key = instruction_.getP2(); // The register must contain a key IdxKey
	int register_number_primary_key = instruction_.getP3();	   // The register must contain a key PKey
	int index_key = registers_[register_number_index_key]->getInteger();
	int primary_key = registers_[register_number_primary_key]->getInteger();
	cursors_[cursor_number].Insert(primary_key, index_key);
}

void DbMachine::CreateIndex() {
	int register_number = instruction_.getP1();
	BTree index_btree(kdb_file_name_);
	registers_[register_number]->setData(index_btree.getRootPage());
}