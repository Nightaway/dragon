#include "maintenance_cache_engine.h"

CacheProcess::CacheProcess()
{

}

CacheProcess::~CacheProcess()
{

}

void CacheProcess::SetPath()
{
	path_ = MCE_APP_PATH;
}

#include <iostream>

void CacheTask::Run(dragon::Cycle &c)
{
	std::cout << "Task Run at " << c.path << std::endl;
}

int main() {
	CacheTask t;
	CacheProcess p;
	p.Init();
	p.PushTask(&t);
	p.Run();
	p.Dispose();
	return 0;
}
