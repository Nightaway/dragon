#include "../Macro.h"
#include "HttpRequest.h"

#include <vector>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

USING_NS_DRAGON

void HttpRequest::parseCookies(const std::string &cookies)
{
	using boost::split;
	using boost::split_iterator;
	using boost::first_finder;
	using boost::copy_range;
	using boost::is_iequal;

	typedef std::vector< std::string > split_vector_type;
    split_vector_type SplitVec;
    split(SplitVec, cookies, boost::is_any_of(";"), boost::token_compress_on); 

	BOOST_FOREACH(std::string &s, SplitVec)
	{
		if (s.empty())
			continue;
		
		split_vector_type KeyValueVec;
		split(KeyValueVec, s, boost::is_any_of("="), boost::token_compress_on ); 
		boost::trim(KeyValueVec[0]);
		if (KeyValueVec.size() == 2)
			cookie_[KeyValueVec[0]] = KeyValueVec[1];
	}
}

void HttpRequest::parseQueryString(const std::string &queryString)
{
	using boost::split;
	using boost::split_iterator;
	using boost::first_finder;
	using boost::copy_range;
	using boost::is_iequal;

	typedef std::vector< std::string > split_vector_type;
    split_vector_type SplitVec;
    split(SplitVec, queryString, boost::is_any_of("&"), boost::token_compress_on); 

	BOOST_FOREACH(std::string &s, SplitVec)
	{
		if (s.empty())
			continue;
		
		split_vector_type KeyValueVec;
		split(KeyValueVec, s, boost::is_any_of("="), boost::token_compress_on ); 
		boost::trim(KeyValueVec[0]);
		if (KeyValueVec.size() == 2)
			queryString_[KeyValueVec[0]] = KeyValueVec[1];
	}
}
