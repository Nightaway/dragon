#include "maintenance_cache_engine.h"

#include "cache_task.h"

NS_USING_DRAGON

MCEProcess::MCEProcess()
{

}

MCEProcess::~MCEProcess()
{

}

void MCEProcess::SetPath()
{
	path_ = MCE_APP_PATH;
}

int main(int argc, char *argv[]) {
	CacheTask t;
	MCEProcess p;
	p.Init();
	p.PushTask(&t);
	p.Run();
	p.Dispose();
	return 0;
}
