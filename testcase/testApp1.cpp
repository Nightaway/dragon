#include <gtest/gtest.h>
#include <dragon.h>

#include "config.h"

using namespace dragon;

class deliveryController : public Controller {
	DECLARE_CONTROLLER(deliveryController)
	DECLARE_ACTION_MAP()

	public:
		void index(QueryString &qs);
};

class testApplication : public Application {
	public:
	virtual void Start();
	virtual void End();

	virtual void SetAppPath();
	virtual void SetAppName();
};

void testApplication::Start()
{
	Application::Start();

	ADD_CONTROLLER(deliveryController)
}

void testApplication::End()
{
	Application::End();
}

void testApplication::SetAppPath()
{
	appPath_ = DRAGON_TEST_PATH"testApp1";
}

void testApplication::SetAppName()
{
	appName_ = "testApp1";
}

////////////////////////  deliveryController ////////////////////

IMPLEMENT_CONTROLLER(deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION(deliveryController, index)
END_ACTION_MAP()

void deliveryController::index(QueryString &qs)
{
	response->StringResult("index");
	response->SetContentType("text/javascript");
}

TEST(testApp1, ad) {
	const char *url   = "/test/ad?index=1";
	const char *ip    = "127.0.0.1";
	const char *host  = "localhost";
	const char *userAgent  = "fx";
	const char *userCookie = "unid=acx0s1";
	const char *acceptLang = "zhCh";

	HttpResponse res;
	HttpRequest  req;
	req.SetMethod(kHttp_Method_Get);
	req.SetUrl(StringRef(url, strlen(url)));
	req.SetIp(StringRef(ip, strlen(ip)));
	req.SetHost(StringRef(host, strlen(host)));
	req.SetUserAgent(StringRef(userAgent, strlen(userAgent)));
	req.SetUserCookie(StringRef(userCookie, strlen(userCookie)));
	req.SetAcceptLanguage(StringRef(acceptLang, strlen(acceptLang)));

	testApplication app;
	app.Start();
	app.ResponseRequest(req, res);
	app.End();

	std::string content = res.GetContent();
	std::string type    = res.GetContentType();
	unsigned code       = res.GetStatusCode();
	ASSERT_STREQ(content.c_str(), "Hello");
	ASSERT_STREQ(type.c_str(), "text/html");
	ASSERT_EQ(code, 200);
}

TEST(testApp1, lg) {
	const char *url   = "/test/lg?index=1";
	const char *ip    = "127.0.0.1";
	const char *host  = "localhost";
	const char *userAgent  = "fx";
	const char *userCookie = "unid=acx0s1";
	const char *acceptLang = "zhCh";

	HttpResponse res;
	HttpRequest  req;
	req.SetMethod(kHttp_Method_Get);
	req.SetUrl(StringRef(url, strlen(url)));
	req.SetIp(StringRef(ip, strlen(ip)));
	req.SetHost(StringRef(host, strlen(host)));
	req.SetUserAgent(StringRef(userAgent, strlen(userAgent)));
	req.SetUserCookie(StringRef(userCookie, strlen(userCookie)));
	req.SetAcceptLanguage(StringRef(acceptLang, strlen(acceptLang)));

	testApplication app;
	app.Start();
	app.ResponseRequest(req, res);
	app.End();

	std::string content = res.GetContent();
	std::string type    = res.GetContentType();
	unsigned code       = res.GetStatusCode();

	ASSERT_STREQ(content.c_str(), "lg");
	ASSERT_STREQ(type.c_str(), "text/html");
	ASSERT_EQ(code, 200);
}

TEST(testApp1, ck) {
	const char *url   = "/test/ck?index=1";
	const char *ip    = "127.0.0.1";
	const char *host  = "localhost";
	const char *userAgent  = "fx";
	const char *userCookie = "unid=acx0s1";
	const char *acceptLang = "zhCh";

	HttpResponse res;
	HttpRequest  req;
	req.SetMethod(kHttp_Method_Get);
	req.SetUrl(StringRef(url, strlen(url)));
	req.SetIp(StringRef(ip, strlen(ip)));
	req.SetHost(StringRef(host, strlen(host)));
	req.SetUserAgent(StringRef(userAgent, strlen(userAgent)));
	req.SetUserCookie(StringRef(userCookie, strlen(userCookie)));
	req.SetAcceptLanguage(StringRef(acceptLang, strlen(acceptLang)));

	testApplication app;
	app.Start();
	app.ResponseRequest(req, res);
	app.End();

	std::string content = res.GetContent();
	std::string type    = res.GetContentType();
	unsigned code       = res.GetStatusCode();

	ASSERT_STREQ(content.c_str(), "ck");
	ASSERT_STREQ(type.c_str(), "text/html");
	ASSERT_EQ(code, 200);
}

TEST(testApp1, index) {
	const char *url   = "/delivery/index?index=1";
	const char *ip    = "127.0.0.1";
	const char *host  = "localhost";
	const char *userAgent  = "fx";
	const char *userCookie = "unid=acx0s1";
	const char *acceptLang = "zhCh";

	HttpResponse res;
	HttpRequest  req;
	req.SetMethod(kHttp_Method_Get);
	req.SetUrl(StringRef(url, strlen(url)));
	req.SetIp(StringRef(ip, strlen(ip)));
	req.SetHost(StringRef(host, strlen(host)));
	req.SetUserAgent(StringRef(userAgent, strlen(userAgent)));
	req.SetUserCookie(StringRef(userCookie, strlen(userCookie)));
	req.SetAcceptLanguage(StringRef(acceptLang, strlen(acceptLang)));

	testApplication app;
	app.Start();
	app.ResponseRequest(req, res);
	app.End();

	std::string content = res.GetContent();
	std::string type    = res.GetContentType();
	unsigned code       = res.GetStatusCode();

	ASSERT_STREQ(content.c_str(), "index");
	ASSERT_STREQ(type.c_str(), "text/javascript");
	ASSERT_EQ(code, 200);
}
