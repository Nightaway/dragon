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
		size_ = capacity_ * sizeof(T *);
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

	T *Get(unsigned idx) {
		assert(idx < capacity_);
		return table_[idx];
	}

	unsigned Capacity() {
		return capacity_;
	}
	
	virtual void Dump(Space &space)
	{
		for (unsigned i=0; i<capacity_; ++i)
			space.Put(table_[i]);
	}

private:
	const unsigned capacity_;
	T **table_;
};

NS_END

#endif
