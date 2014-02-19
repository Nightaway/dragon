#include "HttpRequest.h"

NS_USING_DRAGON

HttpRequest::HttpRequest()
{

}

HttpRequest::~HttpRequest()
{

}

HttpRequest::HttpRequest(HttpMethod method, 
		       StringRef  url,
                       StringRef  ip,
	               StringRef  host,
	               StringRef  userAgent,
	               StringRef  userCookie,
	               StringRef  acceptLanguage) :
	Method_(method), Url_(url), Ip_(ip), Host_(host), UserAgent_(userAgent),
	UserCookie_(userCookie), AcceptLanguage_(acceptLanguage)
{

}
