#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include "../core/Macro.h"
#include "../core/StringRef.h"

#include <map>
#include <string>
#include <sstream>
#include <vector>

NS_DRAGON

typedef enum {
	kResponseTypeNormal = 0,
	kResponseTypeRedirect,
	kResponseTypeDataRef
} ResponseType;

class DataRef {
	unsigned char *data_;
	unsigned       len_;

public:
	DataRef() : data_(0), len_(0) {}
	DataRef(unsigned char *data, unsigned len) : data_(data), len_(len) {}

	unsigned length()
	{
		return len_;
	}

	unsigned char * data()
	{
		return data_;
	}
};

typedef std::map<std::string, std::string> HeaderList;
typedef std::vector<std::string>           CookieList;

class HttpResponse {
	ResponseType Type_;
 	unsigned     StatusCode_;
	std::string  ContentType_;
	std::string  Content_;

	HeaderList Headers_;
	CookieList Cookies_;

	DataRef DataRef_;

public:
	HttpResponse() {}
	~HttpResponse() {}
	
	DataRef GetDetaRef()
	{
		return DataRef_;
	}

	HeaderList GetHeaders()
	{
		return Headers_;
	}

	CookieList GetCookies()
	{
		return Cookies_;
	}

	ResponseType GetResponseType()
	{
		return Type_;
	}
	
	void SetStatusCode(int code)
	{
		StatusCode_ = code;
	}

	unsigned GetStatusCode()
	{
		return StatusCode_;
	}

	void SetContentType(std::string contentType)
	{
		ContentType_ = contentType;
	}

	std::string GetContentType()
	{
		return ContentType_;
	}

	void SetContent(std::string content)
	{
		Content_ = content;
	}

	std::string GetContent()
	{
		return Content_;
	}

	void SetResponse(int statusCode,
			 std::string contentType,
			 std::string contentBody)
	{
		Type_        = kResponseTypeNormal;
		StatusCode_  = statusCode;
		ContentType_ = contentType;
		Content_     = contentBody;
	}

	void StringResult(std::string string)
	{
		Type_        = kResponseTypeNormal;
		StatusCode_  = 200;
		ContentType_ = "text/html";
		Content_     = string;
	}

	void RedirectResult(std::string url)
	{
		Type_        = kResponseTypeRedirect;
		StatusCode_  = 302;
		ContentType_ = "Redirect";
		Content_     = url;
	}

	void DataRefResult(DataRef dataRef)
	{
		Type_        = kResponseTypeDataRef;
		StatusCode_  = 200;
		DataRef_     = dataRef;
	}

	void addHeader(std::string key, std::string value)
	{
		Headers_[key] = value;
	}
		
	void addCookie(std::string value)
	{
		Cookies_.push_back(value);
	}

	void setCookie(std::string key, 
		       std::string value, 
		       std::string expires, 
		       std::string path)
	{
		std::stringstream ss;
		ss << key;
		ss << "=";
		ss << value << "; ";
		if (!expires.empty())
		{
			ss <<"expires=";
			ss << expires << "; ";
		}
		ss << "path=";
		ss << path;
		
		Cookies_.push_back(ss.str());
	}

	void setCookie(std::string key, 
		       std::string value, 
		       std::string expires, 
		       std::string path,
		       std::string domain)
	{
		std::stringstream ss;
		ss << key;
		ss << "=";
		ss << value << "; ";
		if (!expires.empty())
		{
			ss <<"expires=";
			ss << expires << "; ";
		}
		ss << "path=";
		ss << path;
		ss << "; ";
		if (!domain.empty())
		{
			ss << "domain=" << domain;
		}
		Cookies_.push_back(ss.str());
	}
};

NS_END

#endif
