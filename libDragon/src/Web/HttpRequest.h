#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <map>
#include <string>

NS_DRAGON
			typedef enum {
				HTTP = 0,
				HTTPS
			} protocol_e;

			typedef enum {
				GET = 0,
				POST,
				PUT,
				DELETE
			} method_e;

			typedef std::map<std::string, std::string> Cookie;
			typedef std::map<std::string, std::string> QueryString;
			class  HttpRequest {
			private:
				protocol_e protocol_;
				method_e method_;
				std::string IP_;
				std::string routingUrl_;
				std::string userAgent_;
				std::string acceptLanguage_;
				Cookie cookie_;
				QueryString queryString_;

			public:
				HttpRequest(std::string url) : routingUrl_(url) {}

				void parseCookies(const std::string &cookies);
				void parseQueryString(const std::string &queryString);

				//
				protocol_e getProtocol()
				{
					return protocol_;
				}
				void  setProtocol(protocol_e protocol)
				{
					protocol_ = protocol;
				}

				method_e getMethod()
				{
					return method_;
				}
				void  setMethod(method_e method)
				{
					method_ = method;
				}

				const std::string &getIP()
				{
					return IP_;
				}
				void setIP(const std::string &IP)
				{
					IP_ = IP;
				}

				const std::string &getRoutingUrl()
				{
					return routingUrl_;
				}
				void setRoutingUrl(const std::string &url)
				{
					routingUrl_ = url;
				}

				const std::string &getUserAgent()
				{
					return userAgent_;
				}
				void setUserAgent(const std::string &userAgent)
				{
					userAgent_ = userAgent;
				}

				Cookie &getCookie()
				{
					return cookie_;
				}

				QueryString &getQueryString()
				{
					return queryString_;
				}

				 const std::string &getAcceptLanguage()
				 {
					 return acceptLanguage_;
				 }
				void setAcceptLanguage(const std::string &lang)
				{
					acceptLanguage_ = lang;
				}
			};

NS_END

#endif