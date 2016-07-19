#include "cache_task.h"

#include <string>
#include <vector>

#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>

#include <core/Space.h>
#include <core/Table.h>

#include "config.h"

#include <iostream>

NS_USING_DRAGON

std::string Now()
{
  char buf[30];
  time_t now = time(NULL);
  tm *newtime = localtime(&now);
  strftime(buf, 30, "%Y-%m-%d %H:%M:%S", newtime);
  return std::string(buf);
}

NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);

void CacheTask::Run(dragon::Cycle &c)
{
  std::cout << "Task Run at " << c.path << std::endl;

  if (!space.IsCreated())
	  space.Create();
  else
    space.Open();
  space.Switch();

  space.Put("Hello");

  space.Close();
}
