#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <vector>
#include <string>
#include <map>

NS_DRAGON

	typedef struct binary {
		unsigned int      len;
		unsigned char *data;
	} binary_t;

	class HttpResponse {
	private:
		typedef std::map<std::string, std::string> header_map_t;
		typedef std::vector<std::string> cookie_list_t;

		int status_code_;
		std::string content_type_;
		std::string content_;

		header_map_t headers_;
		cookie_list_t cookies_;
		binary_t binary_;

		bool isRedirected_;
		std::string redirectUrl_;

	public:
		HttpResponse() : isRedirected_(false)
		{
			binary_.len   = 0;
			binary_.data = NULL;
		}

		void setStatusCode(int status_code) 
		{
			status_code_ = status_code;
		}
		int getStatusCode()
		{
			return status_code_;
		}

		void setContent(const std::string &content)
		{
			content_ = content;
		}
		const std::string &getContent()
		{
			return content_;
		}

		void setContentType(const std::string &content_type)
		{
			content_type_ = content_type;
		}
		const std::string &getContentType()
		{
			return content_type_;
		}

		void setBinary(unsigned int len, unsigned char *data)
		{
			binary_.len = len;
			binary_.data = data;
		}

		void redirect(const std::string &url) 
		{
			status_code_  = 302;
			isRedirected_ = true;
			redirectUrl_    = url;
		}

		void addHeader(const std::string &key, 
									const std::string &value)
		{
			headers_[key] = value;
		}

		void setCookie(const std::string &key, 
								  const std::string &value, 
								  const std::string &expires, 
								  const std::string &path,
								  const std::string &domain = "");
};

NS_END

#endif
