#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../version.h"
#include "../config.h"
#include "Macro.h"
#include "Module.h"

#include "Modules/Log.h"
#include "Modules/Console.h"
#include "Modules/Config.h"

#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include "../http/UrlRewriting.h"
#include "../http/UrlRouting.h"
#include "../http/HttpProcess.h"

NS_DRAGON

class Application {
protected:
	std::string appPath_;
	std::string appName_;

	JavaScriptCompiler jsCompiler_;
	UrlRewriting urlRewriting_;
	UrlRouting urlRouting_;
	HttpProcess httpProcess_;
	Log logInfo_;
	Log logError_;
	Console console_;
	Config config_;
	 
public:
	Application();
	virtual ~Application();

	virtual void Start();
	virtual void End();

	virtual void SetAppPath();
	virtual void SetAppName();

	void ResponseRequest(HttpRequest &req, HttpResponse &res);
};

NS_END


#endif