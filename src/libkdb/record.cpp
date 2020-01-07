#include "record.h"
//#include "dbm.h"

KdbRecord::KdbRecord(const std::vector<std::shared_ptr<Register>>& registers, int register_number_start, int register_number_end) {
	for (int i = register_number_start;i <= register_number_end;i++) {
		registers_.push_back(registers[i]);
	}
	registers_[0]->


}
KdbRecord::KdbRecord(const std::vector<uint8_t>& raw_data) {

}

int KdbRecord::getType(int column) const {
	RegisterType type = registers_[column]->getType();
	if(type == )
}

int KdbRecord::getInteger(int column) const {
	return registers_[column]->getInteger();
}
const std::string& KdbRecord::getString(int column) const {
	return registers_[column]->getString();
}