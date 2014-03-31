#include "Application.h"

#include "JSCompiler.h"

NS_USING_DRAGON
NS_USING_V8


Application::Application() 
	:       urlRewriting_(jsCompiler_),
		urlRouting_(jsCompiler_),
		httpProcess_(jsCompiler_),
		logInfo_(jsCompiler_, kLogLevelInfo, "info"),
		logError_(jsCompiler_, kLogLevelError, "error"),
		console_(jsCompiler_),
		config_(jsCompiler_)
{

}

Application::~Application()
{

}

void Application::SetAppPath()
{
	appPath_ = "";
}

void Application::SetAppName()
{
	appName_ = "DefaultAppName";
}

void Application::Start()
{
	// #1 Set Application Path
	SetAppPath();

	// #2 Parse configuration file
	std::string configPath = appPath_ + "/conf/App.conf";
	config_.Parse(configPath.c_str());

	// #2 
	std::string logPath = appPath_ + "/logs/info.log";
	logInfo_.Init();
	logInfo_.Open(logPath.c_str());
	logInfo_.LogFmt("Applicaion %s start at %s", appName_.c_str(), appPath_.c_str());

	// #3
	logPath = appPath_ + "/logs/error.log";
	logError_.Init();
	logError_.Open(logPath.c_str());

	// #4
	console_.Init();

	// #5
	jsCompiler_.Load(appPath_, "/scripts");

	// #6
	urlRewriting_.Init();
	urlRouting_.Init();
	httpProcess_.Init();
}

void Application::End()
{
	urlRewriting_.Dispose();
	urlRouting_.Dispose();
	httpProcess_.Dispose();
	console_.Dispose();
	logInfo_.Dispose();
	logError_.Dispose();
	config_.Dispose();
}

void Application::ResponseRequest(HttpRequest &req, HttpResponse &res)
{
	std::string rewritedUrl = urlRewriting_.Rewrite(req.GetUrl());
	req.SetRewritedUrl(rewritedUrl);
	RoutingTable table = urlRouting_.Route(rewritedUrl);
	httpProcess_.Process(table, *this, req, res);
}
