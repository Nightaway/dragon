#include "HttpRequest.h"

#include <vector>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

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

void HttpRequest::ParseCookie()
{
	using boost::split;
	using boost::split_iterator;
	using boost::first_finder;
	using boost::copy_range;
	using boost::is_iequal;
	using boost::trim;

	std::string cookies = std::string(UserCookie_.string(), UserCookie_.length());

	typedef std::vector< std::string > split_vector_type;
    split_vector_type SplitVec;
    split(SplitVec, cookies, boost::is_any_of(";"), boost::token_compress_on); 

	BOOST_FOREACH(std::string &s, SplitVec)
	{
		if (s.empty())
			continue;

		trim(s);
		size_t idx = s.find("=");
		if (idx != std::string::npos) {
			std::string key	    = s.substr(0, idx);
			std::string value   = s.substr(idx+1, s.length());
			cookies_[key] = value;
		}
	}
}
