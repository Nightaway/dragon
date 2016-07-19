#ifndef __MAINTENANCE_CACHE_ENGINE_H__
#define __MAINTENANCE_CACHE_ENGINE_H__

#include "config.h"
#include <dragon.h>

class MCEProcess : public dragon::Process {

public:
	MCEProcess();
	~MCEProcess();

	virtual void SetPath();
};

#endif
