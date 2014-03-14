#include "AdInfo.h"

NS_USING_DRAGON

void AdInfo::Dump(Space &space)
{
	space.Put(id);
	space.Put(name);
	space.Put(text);
	space.Put(width);
	space.Put(height);
}

void AdInfo::Stuff(Space &space)
{
	space.Get(id);
	space.Get(name);
	space.Get(text);
	space.Get(width);
	space.Get(height);
}

unsigned AdInfo::Size()
{
	size_ += sizeof(id);	
	size_ += sizeOf(name);
	size_ += text->Size();
	size_ += sizeof(width);
	size_ += sizeof(height);
	return size_;
}

