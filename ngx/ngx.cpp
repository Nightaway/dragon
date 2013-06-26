#include "Applications/DapApplication.h"

#include <iostream>

int main()
{
	using dragon::HttpRequest;
	using dragon::HttpResponse;
	using dragon::HTTP;
	using dragon::GET;

	DapApplication dap;
	dap.Init();

	const std::string routingUrl = "/Ad/ajs";
	const std::string ip				= "127.0.0.1";
	const std::string userAgent = "IE";
	const std::string lang           = "en-us";
	const std::string cookieString = "PCID=1231231xxx";

	HttpResponse resp;
	HttpRequest req(routingUrl);
	req.setProtocol(HTTP);
	req.setMethod(GET);
	req.setUserAgent(userAgent);
	req.setIP(ip);
	req.setAcceptLanguage(lang);
	req.parseCookies(cookieString);

	dap.Process(req, resp);

	std::cout << resp.getContent() << std::endl;

	dap.Dispose();
	return 0;
}