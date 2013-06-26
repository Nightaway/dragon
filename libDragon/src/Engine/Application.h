#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <boost/foreach.hpp>
#include <v8.h>

#include "../Macro.h"
#include "../RuntimeInfo/RuntimeInfo.h"
#include "../RuntimeInfo/Memory.h"
#include "../RuntimeInfo/TypeMapping.h"
#include "../RuntimeInfo/Object.h"

#include "../Web/HttpRequest.h"
#include "../Web/HttpResponse.h"
#include "Internals/Compiler.h"
#include "../Web/HttpProcess.h"
#include "Config.h"
#include "Router.h"

#define kTagApplication 1

NS_DRAGON 

class Application : 
			public core::Object 
{
protected:
	DECLARE_CLASS(Application);

	std::string path;

public:
	virtual ~Application() {}

	virtual void Init();
	virtual void Dispose();

	inline void Process(HttpRequest &request,
									HttpResponse &response)
	{
		router_.route(request, response);
	}

	// 
	virtual void GetPath();

	/*
			Log		模块 
			Config 模块 
			Router 模块
			View    模块
	*/
	Config config_;
	JavaScriptCompiler jsc_;
	Router router_;
};

NS_END



#endif