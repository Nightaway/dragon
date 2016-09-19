#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <map> 
#include "../core/Macro.h"
#include "../core/StringRef.h"

NS_DRAGON

typedef enum {
	kHttp_Method_Get = 0,
	kHttp_Method_Post,
	kHttp_Method_Put,
	kHttp_Method_Delete
} HttpMethod;

typedef struct {
	unsigned char *data;
	unsigned len;
} Post;

typedef std::pair<std::string, std::string> CookiePair;
typedef std::map<std::string, std::string> Cookie;
typedef std::map<std::string, std::string> HeaderInList;

class HttpRequest {
	HttpMethod Method_;
	StringRef Url_;
	StringRef Ip_;
	StringRef Host_;
	StringRef UserAgent_;
	StringRef UserCookie_;
	StringRef AcceptLanguage_;
	Cookie cookies_;
	Post post_;
	HeaderInList Headers_;

public:
	HttpRequest();
	HttpRequest(HttpMethod method, 
		StringRef  url,
		StringRef  ip,
		StringRef  host,
		StringRef  userAgent,
		StringRef  userCookie,
		StringRef  acceptLanguage);
	~HttpRequest();

	void SetMethod(HttpMethod method)
	{
		Method_ = method;
	}

	HttpMethod GetMethod()
	{
		return Method_;
	}

	void SetUrl(StringRef url)
	{
		Url_ = url;
	}

	StringRef GetUrl()
	{
		return Url_;
	}

	void SetIp(StringRef ip)
	{
		Ip_ = ip;
	}

	StringRef GetIp()
	{
		return Ip_;
	}

	void SetHost(StringRef host)
	{
		Host_ = host;
	}

	StringRef GetHost()
	{
		return Host_;
	}

	void SetUserAgent(StringRef userAgent)
	{
		UserAgent_ = userAgent;
	}

	StringRef GetUserAgent()
	{
		return UserAgent_;
	}

	void SetUserCookie(StringRef userCookie)
	{
		UserCookie_ = userCookie;
	}

	StringRef GetUserCookie()
	{
		return UserCookie_;
	}

	void SetAcceptLanguage(StringRef acceptLanguage)
	{
		AcceptLanguage_ = acceptLanguage;
	}

	StringRef GetAcceptLanguage()
	{
		return AcceptLanguage_;
	}

	Cookie &GetCookie()
	{
		return cookies_;
	}

	void SetPost(Post post)
	{
		post_ = post;
	}

	Post GetPost()
	{
		return post_;
	}

	void ParseCookie();

	HeaderInList &GetHeaders()
	{
		return Headers_;
	}
};


NS_END


#endif
