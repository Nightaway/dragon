#ifndef __TASK_H__
#define __TASK_H__

#include "../core/Macro.h"

NS_DRAGON

struct Task {
	virtual void run() = 0;
};

NS_END


#endif
