#ifndef __QUERY_STRING_H__
#define __QUERY_STRING_H__

#include "Macro.h"

#include <map>
#include <string>

NS_DRAGON

typedef std::map<std::string, std::string> QueryString;

void parse(std::string raw_params,
	   QueryString &args_map,
	   std::string delim);

NS_END

#endif
