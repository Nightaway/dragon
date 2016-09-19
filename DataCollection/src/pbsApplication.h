#ifndef __PBS_APPLICATION_H__
#define __PBS_APPLICATION_H__

#include <dragon.h>

class pbsApplication : 
	public dragon::Application {

	public:
		virtual void Start();
		virtual void End();
		virtual void SetAppPath();

	private:
		
};

#endif
