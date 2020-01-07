#include "dbm-cursor.h"
#include "record.h"
#include "dbm.h"

void Cursor::OpenRead(PageNumber root) {

}
void Cursor::OpenWrite(PageNumber root) {

}
void Cursor::Close() {

}

void Cursor::Insert(int key, const std::vector<uint8_t>& record) {

}
void Cursor::Insert(int primary_key, int index_key) {

}

bool Cursor::ReWind() {

}
bool Cursor::Next() {

}
bool Cursor::Prev() {

}

bool Cursor::Seek(int key) {

}
bool Cursor::SeekGt(int key) {

}
bool Cursor::SeekGe(int key) {

}
bool Cursor::SeekLt(int key) {

}
bool Cursor::SeekLe(int key) {

}

int Cursor::GetKey() const {
	return cell_->getKey();
}
int Cursor::GetIndexKey() const {
	return cell_->getKey();
}
int Cursor::GetIdxPKey() const {
	return std::static_pointer_cast<IndexCell>(cell_)->getPrimaryKey();
}
std::shared_ptr<Register> Cursor::GetColumn(int column_number) const {
	return record_->getColumn(column_number);
}