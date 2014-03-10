#ifndef __TASK_H__
#define __TASK_H__

#include "../core/Macro.h"

NS_DRAGON

struct Task {
	Task() {}
	virtual ~Task() {}
	virtual void Run() {}
};

NS_END


#endif
