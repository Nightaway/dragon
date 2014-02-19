#ifndef __DE_APPLICATION_H__
#define __DE_APPLICATION_H__

#include <dragon.h>

class DEApplication : 
	public dragon::Application {

	public:
		virtual void Start();
		virtual void End();
		virtual void SetAppPath();
};

#endif
