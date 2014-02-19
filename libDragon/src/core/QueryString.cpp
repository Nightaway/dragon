#include "QueryString.h"

#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

NS_USING_DRAGON

void dragon::parse(std::string raw_params,
		  QueryString &args_map,
	       	  std::string delim)
{
	using std::string;
	using std::vector;
	using boost::split_iterator;
	using boost::make_split_iterator;
	using boost::copy_range;
	using boost::first_finder;
	using boost::is_iequal;

	vector<string> params;
	typedef split_iterator<string::iterator> string_split_iterator;
	for(string_split_iterator It=
		make_split_iterator(raw_params, first_finder(delim, is_iequal()));
	     It != string_split_iterator();
		++It)
	{
		params.push_back(copy_range<std::string>(*It));
	}

	std::string::size_type mid_idx;
	delim = "=";
	vector<string>::iterator end =  params.end();
	for (vector<string>::iterator iter = params.begin(); iter != end; ++iter)
	{
		string sKeyValue = *iter;

		mid_idx = sKeyValue.find_first_of(delim);
		if (mid_idx != string::npos)
		{
			string sKey(sKeyValue, 0, mid_idx);
			args_map[sKey] = string(sKeyValue, mid_idx+1, sKeyValue.length());
		}
	}
}
