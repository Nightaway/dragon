#include "String.h"

NS_USING_DRAGON

unsigned dragon::String::Size()
{
  unsigned size = 0;
  size += sizeof(unsigned);
  size += length_;
  return size;
}
