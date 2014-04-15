#ifndef __MOTU_FILTER_H__
#define __MOTU_FILTER_H__

#include <core/QueryString.h>
#include <http/HttpRequest.h>
#include <http/HttpResponse.h>

#define MOTU_CACHE_TIME (60*60*24)

typedef struct {
	std::string name;
	std::string price;
	std::string type;
	std::string car_id;
	std::string api_host;
	std::string httpRequestStr;
} MotuInfo;

class MotuFilter {
	MotuInfo _motuInfo;
		
public:

	bool motu_filter(std::string &limition, std::string &op, std::string &car_id);

	static void init();
	static void dispose();
};

	
#endif