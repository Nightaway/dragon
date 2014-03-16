#ifndef __HEAP_H__
#define __HEAP_H__

#include "Macro.h"
#include "Allocation.h"
#include "Object.h"
#include "String.h"
#include "Space.h"

NS_DRAGON

struct StringObject {
	unsigned char mark;
	String string;
};

class StringHeap : public NamedSemiSpace {
public:
	StringHeap(const char *name, unsigned size);
	~StringHeap();

	Ref<String> Allocate(const char *str, unsigned size);

	void AddRoot(Ref<String> *root)
	{
		roots_[index_++] = root;
	}

	void PrintObjs();

private:
	Ref<String> **roots_;
	unsigned rootCapacity_;
	unsigned index_;
};

NS_END

#endif
