#include "Object.h"

NS_USING_DRAGON

Object::Object()
{

}

Object::~Object()
{

}

Integer::Integer()
{

}
  
Integer::~Integer()
{

}

int Integer::Value()
{
  int *i = reinterpret_cast<int *>(addr_);
  return *i;
}

Ref<Integer> Integer::New(int value, Space &space) 
{
  void **addr = reinterpret_cast<void **>(space.Allocate(sizeof(void *)));
  int *i = reinterpret_cast<int *>(space.Allocate(sizeof(int)));
  memcpy(i, &value, sizeof(int));
  *addr = reinterpret_cast<void *>(i);
  return Ref<Integer>::Cast(addr);
}
