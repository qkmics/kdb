#include "record.h"

KdbRecord::KdbRecord(const std::vector<std::shared_ptr<Register>>& registers, int register_number_start, int register_number_end) {

}
KdbRecord::KdbRecord(const std::vector<uint8_t>& raw_data) {

}
int KdbRecord::getInteger(int column) const {
	return registers_[column]->getInteger();
}
const std::string& KdbRecord::getString(int column) const {
	return registers_[column]->getString();
}