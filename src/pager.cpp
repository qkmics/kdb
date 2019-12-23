#include "pager.h"

#include <iostream>

Pager::Pager(const std::string& file_name, int page_size) : kPageSize(page_size) {
	kdb_file_.open(file_name,std::ios::binary);
}

Pager::~Pager() {
	kdb_file_.close();
}

void Pager::AllocatePage(PageNumber& page_number) {
	page_number = ++number_of_pages_;
}

PageNumber Pager::GetRealDBSize() const {

	int file_length = kdb_file_.tellg();
	return file_length / kPageSize;

}

void Pager::ReadPage(MemPage& mem_page) {
	PageNumber page_number = mem_page.getPageNumber();

	kdb_file_.seekg(page_number * kPageSize);
	std::string data;
	data.resize(kPageSize);
	kdb_file_.read(&data[0], kPageSize);

	mem_page.setRawData(data);
}

void Pager::WritePage(const MemPage& mem_page) {
	PageNumber page_number = mem_page.getPageNumber();

	kdb_file_.seekg((page_number - 1) * kPageSize);
	const std::vector<uint8_t>& raw_data = mem_page.getRawData();
	std::string data;
	data.resize(kPageSize);
	for (int i = 0;i < raw_data.size();i++) {
		data[i] = raw_data[i];
	}
	kdb_file_.write(&data[0], kPageSize);
}

MemPage::~MemPage() {

}

void MemPage::setRawData(const std::string& raw_data) {
	raw_data_.reserve(raw_data.size());
	for (int i = 0;i < raw_data.size();i++) {
		raw_data_[i] = raw_data[i];
	}

}

void MemPage::setRawData(const std::vector<uint8_t>& raw_data) {
	raw_data_ = raw_data;
}