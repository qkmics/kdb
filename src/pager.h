#ifndef PAGER
#define PAGER

#include <fstream>
#include <string>
#include <memory>
#include <vector>

typedef uint16_t CellNumber;
typedef uint32_t PageNumber;
typedef uint32_t KdbKey;

class MemPage {
public:
	MemPage(PageNumber page_number) : page_number_(page_number) {}

	MemPage(PageNumber page_number, const std::string& data);
	~MemPage();

	PageNumber getPageNumber() const{ return page_number_; }

	const std::vector<uint8_t>& getRawData() const{ return raw_data_; }

	void setRawData(const std::vector<uint8_t>& raw_data);

private:
	std::vector<uint8_t> raw_data_;
	PageNumber page_number_;
};


class Pager {
public: 
	Pager(const std::string& file_name, int page_size);
	~Pager();

	void AllocatePage(PageNumber& page_number);

	PageNumber GetRealDBSize() const;

	void ReadPage(std::shared_ptr<MemPage>& mem_page, PageNumber page_number) ;

	void WritePage(const MemPage& mem_page);

private:

	std::fstream kdb_file_;
	PageNumber number_of_pages_;
	const uint16_t kPageSize;
};


#endif //  PAGER
