#ifndef __CACHE_TASK_H__
#define __CACHE_TASK_H__

#include <maintenance/Task.h>
#include <maintenance/Cycle.h>

class CacheTask : public dragon::Task {

public:
	virtual void Run(dragon::Cycle &c);
};

#endif
