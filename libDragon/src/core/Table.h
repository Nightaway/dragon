#ifndef __TABLE_H__
#define __TABLE_H__

#include "Macro.h"
#include "Allocation.h"
#include "Object.h"
#include "Space.h"
#include <memory.h>
#include <assert.h>

NS_DRAGON

template<typename T>
class Table : public Object {
public:
	Table() : capacity_(0), table_(NULL) { 
		size_ = 0;
	}

	Table(unsigned cap) : capacity_(cap) { 
		size_ = capacity_ * sizeof(T **);
		table_ = static_cast<T **>(Malloced::New(size_));
		memset(table_, 0, size_);
	}

	~Table() {
		Malloced::Delete(table_);
	}

	void Put(unsigned idx, T *e) {
		assert(idx < capacity_);
		table_[idx] = e;
	}

	Ref<T> Get(unsigned idx) {
		assert(idx < capacity_);
		return Ref<T>(table_[idx]);
	}

	unsigned Capacity() {
		return capacity_;
	}
	
	void Dump(Space &space)
	{
		for (unsigned i=0; i<capacity_; ++i)
			space.Put(table_[i]);
	}

	void Stuff(Space &space)
	{
		for (unsigned i=0; i<capacity_; ++i)
			space.Get(table_[i]);
	}
private:
	unsigned capacity_;
	T **table_;
};

class OffsetTable {
public:
	OffsetTable();
	~OffsetTable();
	OffsetTable(unsigned capacity, Space &space);

	void Open(Space &space);

	void Put(unsigned id, unsigned offset)
	{
		assert(table_);	
		table_[id] = offset;
	}

	unsigned Get(unsigned id)
	{
		assert(table_);
		return table_[id];
	}

	void Dump(Space &space)	
	{
		for (unsigned i=0; i<capacity_; ++i)
			space.Put(table_[i]);
	}

	void Stuff(Space &space)
	{
		for (unsigned i=0; i<capacity_; ++i)
			space.Get(table_[i]);
	}

	unsigned GetCapacity()
	{
		return capacity_;
	}

	unsigned *GetTable()
	{
		return table_;
	}

private:
	unsigned capacity_;
	unsigned *table_;
};

NS_END

#endif
