#ifndef DBM
#define DBM

#include "dbm-cursor.h"
#include "btree.h"

#include <vector>
#include <string>
#include <memory>

enum class RegisterType {
	kUnspecified, kNull, kInt32, kString, kBinary
};


enum class InstructionType {
	kInteger, kString, kNull, kSCopy,					// Register Manipulation Instructions
	kEq, kNe, kLt, kLe, kGt, kGe, kHalt, kNoop,			// Control Flow Instructions
	kOpenRead, kOpenWrite, kClose,						// Database Opening and Closing Instructions
	kRewind, kNext, kPrev, kSeek, kSeekGt, kSeekGe, 
	kSeekLt, kSeekLe, kIdxGt, kIdxGe, kIdxLt, kIdxLe,	// Cursor Manipulation Instructions
	kColumn, kKey, kIdxPKey,							// Cursor Access Instructions
	kMakeRecord, kResultRow,							// Database Record Instructions
	kInsert, kIdxInsert,								// Insertion Instructions
	kCreateTable, kCreateIndex							// B-Tree Creation Instructions
};

class Register  {
 public:
	
	Register() {}

	void setData(void);

	void setData(int integer) { integer_ = integer; }

	void setData(const std::string& string_data) { string_ = string_data; }

	void setData(const std::vector<uint8_t>& bytes) { bytes_ = bytes; }

	int Compare(const Register&) const { return 0; }

	int getInteger() const;

	const std::string& getString() const;

	const std::vector<uint8_t>& getBytes() const;

	void operator= (const Register& reg);

 private:
	RegisterType type_;
	int integer_;
	std::string string_;
	std::vector<uint8_t> bytes_;
};


/*
class IRgeister {
public:
	virtual void getData() = 0;
	
	void setType(RegisterType type) { type_ = type; }
	RegisterType getType() { return type_; }
private:
	RegisterType type_;
};

template<typename T> 
class Register : public IRgeister {
public:
	Register(const T& data) : data_(data) {}
	const T& getData() { return data_; }
	void setData(const T& data) { data_ = data; }
	
	const T& getData() {return data_}


private:
	T data_;

	
	int integer_;
	std::string string_;
	std::vector<uint8_t> bytes_;
};
*/
class Instruction {
public:
	Instruction(InstructionType type, int p1, int p2, int p3, const std::string& p4) : type_(type), p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}

	int getP1() { return p1_; }
	int getP2() { return p2_; }
	int getP3() { return p3_; }
	const std::string& getP4() { return p4_; }

	InstructionType getType() { return type_; }

private:
	InstructionType type_;
	int p1_;
	int p2_;
	int p3_;
	std::string p4_;
};

class DbMachine {
public:

	void Run();

private:
	std::unique_ptr<BTree> btree;

	uint32_t pc_;
	std::vector<Instruction> instructions_;
	Instruction& instruction_;
	
	std::vector<std::shared_ptr<Register>> registers_;

	std::vector<Cursor> cursors_;

	// Result Row: First register and number of registers 
	uint32_t result_row_start_register_;
	uint32_t result_row_size_;

	// Result row: column names 
	// Size of cols_ is determined by the SQL statement. result_row_size_ should always
	// match this number of columns. 
	std::vector<std::string> cols_;


	std::string kdb_file_name_;
	
	// Register Manipulation Instructions
	void Integer();
	void String();
	void Null();
	void SCopy();

	// Control Flow Instructions
	void Eq();
	void Ne();
	void Lt();
	void Le();
	void Gt();
	void Ge();
	void Halt();

	// Database Opening and Closing Instructions
	void OpenRead();
	void OpenWrite();
	void Close();

	// Cursor Manipulation Instructions
	void Rewind();
	void Next();
	void Prev();
	void Seek();
	void SeekGt();
	void SeekGe();
	void SeekLt();
	void SeekLe();

	// SELECT queries
	void ResultRow();
	void Column();
	void Key();

	// INSERT statements
	void MakeRecord();
	void Insert();

	// CREATE TABLE statements
	void CreateTable();

	// Index operations
	void IdxGt();
	void IdxGe();
	void IdxLt();
	void IdxLe();
	void IdxPKey();
	void IdxInsert();
	void CreateIndex();

};


#endif // DBM