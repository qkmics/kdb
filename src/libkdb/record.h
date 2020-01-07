#ifndef RECORD
#define RECORD

#include "dbm.h"

enum class RecordFieldType {
	kNull,			//
	kByte,			// int8
	kSmallInt,		// int16
	kInteger,		// int32
	kText			// raw-string(n)
};


class KdbRecord {
public:
	KdbRecord(const std::vector<std::shared_ptr<Register>>& registers, int register_number_start, int register_number_end);
	KdbRecord(const std::vector<uint8_t>& raw_data);
	~KdbRecord();

	std::shared_ptr<Register> getColumn(int column) const { return registers_[column]; }

	const std::vector<uint8_t>& getRawData() const { return raw_data_; }

	//int getType(int column) const;
	int getInteger(int column) const;
	const std::string& getString(int column) const;


private:
	std::vector<uint8_t> raw_data_;
	std::vector<std::shared_ptr<Register>> registers_;
};

#endif //  RECORD