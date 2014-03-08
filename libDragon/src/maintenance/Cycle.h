#ifndef __CYCYLE_H__
#define __CYCYLE_H__

#include "../core/Macro.h"

NS_DRAGON

struct Cycle {
	pid_t master_pid_;
	pid_t woker_pid_;
};


NS_END

#endif
