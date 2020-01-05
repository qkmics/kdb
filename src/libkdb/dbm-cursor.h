#ifndef CURSOR
#define CURSOR

#include "btree.h"

#include <vector>
#include <string>
#include <memory>

enum class CursorType {
	kUnspecified, kRead, kWrite
};

class Register;
class KdbRecord;

class Cursor {
public:
	void OpenRead(PageNumber root);
	void OpenWrite(PageNumber root);
	void Close();

	// Insert to where the cursor point to
	void Insert(int key, const std::vector<uint8_t>& record);
	void Insert(int primary_key,int index_key);

	// Get the values from the entry where the cursor point to
	int GetKey() const;
	int GetIndexKey() const;
	int GetIdxPKey() const;

	std::shared_ptr<Register> GetColumn(int column_number) const;

	bool ReWind();
	bool Next();
	bool Prev();

	bool Seek(int key);
	bool SeekGt(int key);
	bool SeekGe(int key);
	bool SeekLt(int key);
	bool SeekLe(int key);

private:
	std::shared_ptr<BTreeCell> cell_;
	CursorType type_;
	std::shared_ptr<BTree> btree_;

	std::shared_ptr<KdbRecord> record_;
};


#endif // CURSOR