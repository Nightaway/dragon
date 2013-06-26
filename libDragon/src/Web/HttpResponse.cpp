#include "../Macro.h"
#include "HttpResponse.h"

USING_NS_DRAGON;

void HttpResponse::setCookie(const std::string &key, 
												  const std::string &value, 
												  const std::string &expires, 
												  const std::string &path,
												  const std::string &domain)
{
	std::string v;
	v += key + "=" + value + "; ";

	if (!expires.empty())
	{
		v += "expires=";
		v += expires + ";";
	}
	v += "path=" + path + "; ";
	if (!domain.empty())
	{
		v  += "domain=" + domain;
	}
	cookies_.push_back(v);
}