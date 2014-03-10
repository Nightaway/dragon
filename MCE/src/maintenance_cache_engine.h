#ifndef __MAINTENANCE_CACHE_ENGINE_H__
#define __MAINTENANCE_CACHE_ENGINE_H__

#include "config.h"
#include <dragon.h>


class CacheProcess : public dragon::Process {

public:
	CacheProcess();
	~CacheProcess();

	virtual void SetPath();
};

class CacheTask : public dragon::Task {

public:
	virtual void Run();
};

#endif
