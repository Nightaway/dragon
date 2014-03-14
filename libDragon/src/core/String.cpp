#include "String.h"

NS_USING_DRAGON

unsigned dragon::String::Size()
{
	size_ = length_;
	return size_;
}
