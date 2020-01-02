#include "dbm-cursor.h"

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

int Cursor::GetIndexKey() {
	
}
int Cursor::StoreColumn(int column_number, std::shared_ptr<Register> reg) {
	
}
int Cursor::StoreKey(std::shared_ptr<Register> reg) {
	reg->setData(cell_->getKey());
}
int Cursor::StoreIdxPKey(std::shared_ptr<Register> reg) {

}