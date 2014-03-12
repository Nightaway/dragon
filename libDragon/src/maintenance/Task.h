#ifndef __TASK_H__
#define __TASK_H__

#include "../core/Macro.h"

#include "Cycle.h"

NS_DRAGON

struct Task {
	Task() {}
	virtual ~Task() {}
	virtual void Run(Cycle &c) {}
};

NS_END


#endif
