#include "AdInfo.h"

NS_USING_DRAGON

void AdInfo::Dump(Space &space)
{
	space.Put(id);
	space.Put(name);
	space.Put(width);
	space.Put(height);
}
