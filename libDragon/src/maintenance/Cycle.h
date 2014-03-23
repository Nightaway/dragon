#ifndef __CYCYLE_H__
#define __CYCYLE_H__

#include <unistd.h>
#include "../core/Macro.h"
#include "../core/modules/Config.h"
#include "../core/modules/Log.h"

NS_DRAGON

struct Cycle {
	const char *path;
	pid_t master_pid;
	pid_t worker_pid;
	Config &config;
	Log    &logInfo;
	Log    &logError;
};

NS_END

#endif
