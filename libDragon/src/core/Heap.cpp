#include "Heap.h"

#include <stdio.h>

NS_USING_DRAGON

StringHeap::StringHeap(const char *name, 
		       unsigned size) : 
			NamedSemiSpace(name, size),
			rootCapacity_(512),
			index_(0)
{
	int bytes = rootCapacity_ * sizeof(Ref<String> *);
	roots_ = reinterpret_cast<Ref<String> **>(Malloced::New(bytes));
	memset(roots_, 0, bytes);
}

StringHeap::~StringHeap()
{
	Malloced::Delete(roots_);
}

Ref<String> StringHeap::Allocate(const char *c_str, unsigned size)
{
	Ref<StringObject> strObj = Ref<StringObject>::Cast(Space::Allocate(sizeof(StringObject)));
	strObj->mark = 0;
	Ref<String> str = String::New(c_str, size, *this);
	strObj->string = *str;
	return str;
}

void StringHeap::PrintObjs()
{
	Ref<StringObject> obj = Ref<StringObject>::Cast(addr_);
	unsigned size = sizeof(StringObject);
	for (; !obj.IsNull() && obj->string.GetLength()!=0; obj = Ref<StringObject>::Cast(addr_ + size))
	{
		printf("mark:%d, len:%d, str:%s\n", obj->mark, obj->string.GetLength(), obj->string.GetString());
	}
}
