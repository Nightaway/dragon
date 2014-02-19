#ifndef __URL_ROUTING_H__
#define __URL_ROUTING_H__

#include "../config.h"
#include "../core/Macro.h"
#include "../core/Module.h"
#include "../core/StringRef.h"

#include <string>

NS_DRAGON

struct RoutingTable {
	std::string ControllerName;
	std::string ActionName;
	std::string QueryString;
};

struct IUrlRouting {
	virtual RoutingTable Route(std::string Url) = 0;
};

class UrlRouting : 
               public JavaScriptModule, public IUrlRouting {
	JavaScriptSource *pSource_;

public:
	UrlRouting(JavaScriptCompiler &jsCompiler);

	virtual void Init();
	virtual void Dispose();

	virtual RoutingTable Route(std::string Url);
};

NS_END

#endif
