#include "../Filter.h"

#include <string>
#include <set>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include "../../Models/VisitorInfo.h"

using namespace dragon;
using namespace boost::posix_time;
using namespace boost::gregorian;

#define DATE_1970 "Thu, 01-Jan-1970 00:00:01 GMT"

typedef struct {
	int count;
	time_t time;
} cookie_count_pair_t;

typedef struct {
	int id;
	int count;
	cookie_count_pair_t timeWeek;
	std::vector<cookie_count_pair_t> timeDays;
} cookie_count_t;

inline int GetIdInTag(std::string prefix, std::string flag)
{
	prefix = prefix + "[";
	std::string::size_type posPrefix = flag.find(prefix);
	if (posPrefix != std::string::npos) {
		std::string::size_type posClosed = flag.find("]", posPrefix);
		if (posClosed != std::string::npos) {
			std::string::size_type posId = posPrefix + 5;
			std::string id = flag.substr(posId, posClosed - posId);
			return boost::lexical_cast<int>(id);
		} else {
			return -1;
		}
	}  else {
		return -1;
	}
}

inline void GetImpFlag(std::set<int> &set, const Cookie &cookie, const std::string &prefix)
{
	typedef std::pair<std::string, std::string> cookie_pair_t;
	BOOST_FOREACH(cookie_pair_t p, cookie)
	{
		int id = GetIdInTag(prefix, p.first);
		if (id != -1) {
			set.insert(id);
		}
		
	}
}

inline std::string GenTag(int id, const std::string prefix)
{
	std::string strFlag = prefix;
	strFlag += "[";
	strFlag += boost::lexical_cast<std::string>(id);
	strFlag += "]";
	return strFlag;
}

inline void DeleteImpFlag(std::set<int> &set, HttpResponse *response,const std::string &prefix)
{
	BOOST_FOREACH(int id, set)
	{
		response->setCookie(GenTag(id, prefix), "deleted", DATE_1970, "/");
	}
}

bool CookieFilter(AdInfo *adInfo, Information &infos) {
	std::set<int> impc_ids;
	std::set<int> impb_ids;
	GetImpFlag(impc_ids, infos.req->GetCookie(), "impc");
	GetImpFlag(impb_ids, infos.req->GetCookie(), "impb");

	DeleteImpFlag(impc_ids, infos.res, "impc");
	DeleteImpFlag(impb_ids, infos.res, "impb");
	return false;
}
                  
