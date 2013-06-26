#include "Application.h"

USING_NS_DRAGON
USING_NS_CORE

IMPLEMENT_CLASS(kTagApplication, Object, Application)

void Application::Init()
{
	/*
			获取 应用程序路径
	*/
	GetPath();

	const std::string cfgPath = path + "App.config";
	config_.Parse(cfgPath.c_str());

	jsc_.Load(path);
	jsc_.LoadLibs(path);

	router_.setApp(this);
}

void Application::Dispose()
{

}

void Application::GetPath()
{
	this->path = "";
}