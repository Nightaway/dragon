#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../config.h"
#include "Macro.h"
#include "Module.h"

#include "Log.h"

#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include "../http/UrlRewriting.h"
#include "../http/UrlRouting.h"
#include "../http/HttpProcess.h"

NS_DRAGON

class Application {
protected:
	std::string appPath_;

	JavaScriptCompiler jsCompiler_;
	UrlRewriting urlRewriting_;
	UrlRouting urlRouting_;
	HttpProcess httpProcess_;
	Log log_;
	 
public:
	Application();
	~Application();

	virtual void Start();
	virtual void End();

	virtual void SetAppPath();

	void ResponseRequest(HttpRequest &req, HttpResponse &res);
};

NS_END


#endif
