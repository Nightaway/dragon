#ifndef __MODULE_H__
#define __MODULE_H__

#include "../config.h"
#include "Macro.h"
#include "JSCompiler.h"

NS_DRAGON

class Module {
protected:

	
public:
	Module()  {}
	virtual ~Module();

	virtual void Init();
	virtual void Dispose();
};

class JavaScriptModule :
		public Module {
protected:
	JavaScriptCompiler &jsCompiler_;
	
public:
	JavaScriptModule(JavaScriptCompiler &jsCompiler) :
			jsCompiler_(jsCompiler) {}
	
};

NS_END


#endif
