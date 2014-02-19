#include <iostream>
#include <cstring>

#include <DEApplication.h>

using namespace dragon;

int main(int argc, char *argv[])
{
	const char *url   = "/delivery/ad?index=1";
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

	DEApplication app;

	try {
		app.Start();

		app.ResponseRequest(req, res);

		app.End();

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
