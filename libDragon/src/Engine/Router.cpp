#include "Application.h"
#include "Router.h"

USING_NS_DRAGON;
USING_NS_V8;

std::map<std::string, std::string> Router::routingMap;

void Router::Init()
{
	routingMap["/dap/www/delivery/ajs.php"]       = "/delivery/ajs";
	routingMap["/dap/www/delivery/rpl.php"]       = "/delivery/rpl";
	routingMap["/dap/www/delivery/exp.php"]      = "/delivery/exp";
	routingMap["/dap/www/delivery/rpls.php"]      = "/delivery/rpls";
	routingMap["/dap/www/delivery/mmt.php"]    = "/delivery/mmt";
	routingMap["/dap/www/delivery/lg.php"]        = "/delivery/lg";
	routingMap["/dap/www/delivery/ck.php"]       = "/delivery/ck";
}

bool Router::buildRoutingTable(routing_table_t *rt, std::string url)
{
	typedef std::pair<std::string, std::string> UrlPair;
	BOOST_FOREACH(const UrlPair &urlPair, routingMap)
	{
		size_t pos = url.find(urlPair.first);
		if (pos != std::string::npos)
		{
			url.replace(pos, urlPair.first.length(), urlPair.second);
		}
	}

	std::string defaultControllerName    = "HomeController";
	std::string defaultActionName          = "Index";
	std::string defaultOptionParameter   = "$default";
	std::string defaulParameters              = "$default";

	// URL 为 '/' ，创建默认Controller 
	if (url.length() == 0 || (url[0] == '/' && url.length() == 1))
	{
		rt->ControllerName    = "defaultControllerName";
		rt->ActionName          = "defaultActionName";
		rt->OptionParameter  = "defaultOptionParameter";
		rt->Parameters            = "defaulParameters";
		return true;
	}

	std::string::iterator iter = url.begin() + 1;
	std::string::iterator end = url.end();
	while (iter != end && *iter != '/')  {
			rt->ControllerName += *iter;
			iter++;
	}

	rt->ControllerName += "Controller";
	// URL 为 '/{Controller}' or '/{Controller}/'
	if (iter == end || (iter+1) == end) {
		rt->ActionName = defaultActionName;
		rt->Parameters   = defaulParameters;
		rt->OptionParameter = defaultOptionParameter;
		return true;
	}

	// eat '/'
	++iter;
	do {
			rt->ActionName += *iter;
			iter++;
			if (iter != end && *iter == '?') {
				// eat '?'
				++iter;
				do {
						rt->Parameters += *iter;
						iter++;
				} while (iter != end);// && *iter != '/');
				// URL 为 '/{Controller}/{Action?parameters}' or '/{Controller}/{Action?parameters}/' 
				if (iter == end || (iter+1) == end) {
						//rt->OptionParameter = "$default";
						return true;
				}
			}
	} while (iter != end && *iter != '/');

	if (iter == end || (iter+1) == end) {
			rt->OptionParameter = defaultOptionParameter;
			return true;
	}

	// eat '/'
	++iter;
	do {
		rt->OptionParameter += *iter;
		iter++;
	} while (iter != end && *iter != '/');

	// URL 为 '/{Controller}/{Action}/{OptionParamater}/' or // URL 为 '/{Controller}/{Action}/{OptionParamater}'
	if (iter == end || (iter+1) == end) {
		/*
					是否 整数类型
		*/
		char *pEnd;
		const char *pOParam = rt->OptionParameter.c_str();
		strtol(pOParam, &pEnd, 0);
		if (*pEnd != '\0') {
				rt->ActionName += "_UNKNOWN";
		} else {
				rt->ActionName += "_I";
		}
		return true;
	} else {
		return false;
	}
}

void Router::route(HttpRequest &request, HttpResponse &response)
{
		routing_table_t rt;
		buildRoutingTable(&rt, request.getRoutingUrl());
		JavaSciprtHttpProcess process(app_->jsc_);
		process.process(request, response, rt.ControllerName, rt.ActionName, rt.Parameters);
}