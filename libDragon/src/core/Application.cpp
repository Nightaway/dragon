#include "Application.h"

#include "JSCompiler.h"

NS_USING_DRAGON
NS_USING_V8

Application::Application() 
	:       urlRewriting_(jsCompiler_),
		urlRouting_(jsCompiler_),
		httpProcess_(jsCompiler_)
{

}

Application::~Application()
{

}

void Application::SetAppPath()
{
	appPath_ = "";
}

void Application::Start()
{
	// #1
	SetAppPath();

	// #2 
	std::string logPath = appPath_ + "/logs/debug.log";
	log_.Open(logPath.c_str());
	log_.LogFmt("App Path: %s", appPath_.c_str());

	jsCompiler_.Load(appPath_, "/Scripts");

	urlRewriting_.Init();
	urlRouting_.Init();
	httpProcess_.Init();
}

void Application::End()
{
	urlRewriting_.Dispose();
	urlRouting_.Dispose();
	httpProcess_.Dispose();
}

void Application::ResponseRequest(HttpRequest &req, HttpResponse &res)
{
	std::string rewritedUrl = urlRewriting_.Rewrite(req.GetUrl());
	req.SetRewritedUrl(rewritedUrl);
	RoutingTable table = urlRouting_.Route(rewritedUrl);
	httpProcess_.Process(table, req, res);
}
