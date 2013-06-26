#ifndef __DAP_APPLICATION_H__
#define __DAP_APPLICATION_H__

#include <Engine/Application.h>

class DapApplication :
		public dragon::Application {

public:
	DapApplication(){}
	virtual ~DapApplication() {}

	virtual void GetPath();
};

#endif