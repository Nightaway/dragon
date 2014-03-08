#ifndef __MODULE_H__
#define __MODULE_H__

#include "../config.h"
#include "Macro.h"
#include "JSCompiler.h"

NS_DRAGON

class Module {
protected:
	const char *_name_;
	short _v_major_;
	short _v_minor_;
	
public:
	Module() : _name_("unnamed"), _v_major_(-1), _v_minor_(-1) {}
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

	virtual v8::Handle<v8::Object> Wrap()  { return v8::Handle<v8::Object>(); }
};

NS_END


#endif
