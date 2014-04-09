#include "../Filter.h"

#include <string>
#include <set>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <cppconn/connection.h>
#include <cppconn/exception.h>

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <db/MysqlContext.h>
#include <utility/DateTime.h>
#include "../../Models/VisitorInfo.h"

using namespace dragon;
using namespace boost::posix_time;
using namespace boost::gregorian;

#define DATE_1970 "Thu, 01-Jan-1970 00:00:01 GMT"

typedef struct {
	int count;
	time_t time;
} cookie_count_t;

typedef struct {
	int id;
	int count;
	cookie_count_t timeWeek;
	std::vector<cookie_count_t> timeDays;
} cookie_count_all_t;

typedef std::pair<int, cookie_count_all_t> count_pair_t;

inline int GetIdInTag(std::string prefix, std::string flag)
{
	prefix = prefix + "[";
	std::string::size_type posPrefix = flag.find(prefix);
	if (posPrefix != std::string::npos) {
		std::string::size_type posClosed = flag.find("]", posPrefix);
		if (posClosed != std::string::npos) {
			std::string::size_type posId = posPrefix + prefix.length();
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
	BOOST_FOREACH(CookiePair p, cookie)
	{
		int id = GetIdInTag(prefix, p.first);
		if (id != -1) {
			set.insert(id);
		}
		
	}
}

inline std::string GenFlag(int id, const std::string prefix)
{
	std::string strFlag = prefix;
	strFlag += "[";
	strFlag += boost::lexical_cast<std::string>(id);
	strFlag += "]";
	return strFlag;
}

inline void DeleteImpFlag(std::set<int> &set, HttpResponse *response, const std::string &prefix)
{
	BOOST_FOREACH(int id, set)
	{
		response->setCookie(GenFlag(id, prefix), "deleted", DATE_1970, "/");
	}
}

inline void DeleteImpFlag(int id, HttpResponse *response, const std::string &prefix)
{
	response->setCookie(GenFlag(id, prefix), "deleted", DATE_1970, "/");
}

inline void UpdateFlag(std::map<int, cookie_count_all_t> &counts)
{
	std::map<int, cookie_count_all_t>::iterator iter = counts.begin();
	for (; iter != counts.end(); ++iter)
	{
		iter->second.count += 1;
	}
}

void GetCountFlag(std::map<int, cookie_count_all_t> &counts, const Cookie &cookie, const std::string &prefix)
{
	BOOST_FOREACH(CookiePair p, cookie)
	{
		int id = GetIdInTag(prefix, p.first);
		printf("id:%d\n", id);
		if (id != -1) {
			std::string value = p.second;
			printf("value:%s\n", value.c_str());

			cookie_count_all_t cc;
			cc.id = boost::lexical_cast<int>(id);
   
   			// Get current count from cookie
    		std::vector<std::string> SplitVec; 
   			boost::split(SplitVec, value, boost::is_any_of("|"), boost::token_compress_on ); 
			int count = boost::lexical_cast<int>(SplitVec[0]);
			cc.count = count;

			// Split count
			std::vector<std::string> ItemVec; 
   			boost::split(ItemVec, SplitVec[1], boost::is_any_of("__"), boost::token_compress_on); 

   			// Get week count
			std::string::size_type pos1 = ItemVec[0].find(".");
			std::string strWeekCount = ItemVec[0].substr(0, pos1);
			std::string strWeekTime  = ItemVec[0].substr(pos1 + 1, ItemVec[0].length() - pos1 - 1);
			int week_count = boost::lexical_cast<int>(strWeekCount);
			time_t week_time = boost::lexical_cast<time_t>(strWeekTime);
			cc.timeWeek.count = week_count;
			cc.timeWeek.time  = week_time;	
			std::cout << "strWeekCount:" << strWeekCount << std::endl;
			std::cout << "strWeekTime:" << strWeekTime << std::endl;
			
			// Get day count
			std::vector<std::string> DaysItemVec; 
   			boost::split(DaysItemVec, ItemVec[1], boost::is_any_of(","), boost::token_compress_on); 
			BOOST_FOREACH(std::string item, DaysItemVec)
			{
				if (item.empty()) 
					continue;

				std::cout << item << std::endl;
				std::string::size_type pos1 = item.find(".");
				std::string strCount = item.substr(0, pos1);
				std::string strTime  = item.substr(pos1 + 1, item.length() - pos1 - 1);
				std::cout << "strDaysCount:" << strCount << std::endl;
				std::cout << "strDyasTime:" << strTime << std::endl;

				int item_count = boost::lexical_cast<int>(strCount);
				time_t item_time = boost::lexical_cast<time_t>(strTime);
				cookie_count_t item_pair;
				item_pair.count = item_count;
				item_pair.time  = item_time;
				cc.timeDays.push_back(item_pair);
			}

			printf("---------- Save id %d ------------\n", cc.id);
			//	counts[cc.id] = cc;
			counts[cc.id] = cc;
		}
	}
}

ptime NextWeekdayFromNow() {
	std::time_t std_now = time(NULL);
	ptime ptime_now = from_time_t(std_now);
	date d = ptime_now.date();
	greg_weekday gw(Sunday);
	days ds = days_until_weekday(d, gw); 
	date nd = next_weekday(d, greg_weekday(Monday));
	date sunday = d + ds;
	days oneDay(1);
	ptime next_dt(sunday + oneDay);
	return next_dt;
}

cookie_count_all_t InitCountFromMysql(VisitorInfo &visitorInfo, sql::Connection *conn)
{
	cookie_count_all_t count;
	std::time_t std_now = time(NULL);
	ptime ptime_now = from_time_t(std_now);
	BOOST_FOREACH(Visitor info, visitorInfo.GetInfos())
	{
		ptime startTime = time_from_string(info.startDate);
		if (info.type == "w") {
				ptime next_dt = NextWeekdayFromNow();
				std::cout << "Weekend day:" << next_dt << std::endl;
			
				tm pt_tm = to_tm(next_dt);
				time_t tt = mktime(&pt_tm);
				
				count.id    = visitorInfo.GetId();
				count.count = 1;
				count.timeWeek.count = info.count;
				count.timeWeek.time  = tt;
			} else if (info.type == "d") {
				days ds(info.time / 24);
				std::cout << "days:" << ds << std::endl;
	
				date endDate   = startTime.date();
				date d = ptime_now.date();
				while (endDate <= d) {
					endDate += ds;	
				}
				std::cout << "end date:" << endDate << std::endl;

				tm pt_tm = to_tm(endDate);
				time_t tt = mktime(&pt_tm);
				cookie_count_t cpair = {info.count, tt};
				count.count = 1;
				count.timeDays.push_back(cpair);
		}
	}
	return count;
}

void InitCountFlag(int id, std::map<int, cookie_count_all_t> &counts, const std::string &prefix, sql::Connection *conn) 
{
	assert(conn);
	if (prefix == "countb") {
		VisitorInfo visitorInfo;
		visitorInfo.Stuff(conn, id, kFilterTypeBanner);
		if (visitorInfo.GetId() != -1) {
			printf("banner id:%d\n", visitorInfo.GetId());
			cookie_count_all_t count = InitCountFromMysql(visitorInfo, conn);
			counts[id] = count;
		}
	} else if (prefix == "countc") {
		VisitorInfo visitorInfo;
		visitorInfo.Stuff(conn, id, kFilterTypeCampaign);
		if (visitorInfo.GetId() != -1 ) {
			printf("campaign id:%d\n", visitorInfo.GetId());	
			cookie_count_all_t count = InitCountFromMysql(visitorInfo, conn);
			counts[id] = count;
		}
	}	
}

inline std::string genKey(int id, const std::string &prefix)
{
	std::string key = prefix + "[";
	key += boost::lexical_cast<std::string>(id);
	key += "]";
	return key;
}

inline std::string genValue(const cookie_count_all_t &count)
{
	std::string value;
	value += boost::lexical_cast<std::string>(count.count);
	value += "|";
	value += boost::lexical_cast<std::string>(count.timeWeek.count);
	value += ".";
	value += boost::lexical_cast<std::string>(count.timeWeek.time);
	value += "__";
	BOOST_FOREACH(cookie_count_t cp, count.timeDays)
	{
		value += boost::lexical_cast<std::string>(cp.count);
		value += ".";
		value += boost::lexical_cast<std::string>(cp.time);
		value += ",";
	}
	return value;
}

void SetFlag(const std::map<int, cookie_count_all_t> &counts, HttpResponse *response, const std::string &prefix)
{
   	typedef std::pair<int, cookie_count_all_t> pair_t;
	BOOST_FOREACH(pair_t p, counts)
	{
		printf("bannerid id:%d\n", p.first);
		std::string key = genKey(p.first, prefix);
		printf("key : %s\n", key.c_str());

		cookie_count_all_t &count = p.second;
		std::string value = genValue(count);
		std::cout << "value:" << value << std::endl;

		response->setCookie(key, value, DateTime::YearFromNow(1), "/");
	}
}

bool CheckFlag(std::map<int, cookie_count_all_t> &counts, int id)
{
	time_t now = time(NULL);
	if (counts.find(id) != counts.end()) 
	{
		cookie_count_all_t cc = counts[id];
		//std::cout << "time_week:" << cc.timeWeek.time << std::endl;

		if (cc.timeWeek.count != 0  && now <= cc.timeWeek.time && cc.count >= cc.timeWeek.count)
			return true;

		BOOST_FOREACH(cookie_count_t &p, cc.timeDays)
		{	
		//std::cout << "cnt:" << p.count << std::endl;

			if (now <= p.time && cc.count >= p.count)
				return true;
		}
	}
	return false;
}

bool VisitorFilter(AdInfo *adInfo, Information &infos) {
	std::set<int> impc_ids;
	std::set<int> impb_ids;
	std::map<int, cookie_count_all_t> countc;
	std::map<int, cookie_count_all_t> countb;
	MysqlContext mysqlCtx;
	sql::Connection *conn = mysqlCtx.Connect("localhost", "root", "", "dap");

	printf("ad id : %d, campaign id : %d\n", adInfo->banner_id, adInfo->campaign_id);

	GetImpFlag(impc_ids, infos.req->GetCookie(), "impc");
	GetImpFlag(impb_ids, infos.req->GetCookie(), "impb");

	GetCountFlag(countc, infos.req->GetCookie(), "countc");
	GetCountFlag(countb, infos.req->GetCookie(), "countb");

	BOOST_FOREACH(count_pair_t p, countb)	{
		printf("count banner id:%d size:%d\n", p.first, countb.size());
	}

	BOOST_FOREACH(count_pair_t p, countc)	{
		printf("count campaign id:%d size:%d\n", p.first, countb.size());
	}

	bool ret = false;
	bool is_found_imp_banner_flag   = false;
	bool is_found_imp_campagin_flag = false;
	if (impc_ids.find(adInfo->campaign_id) != impc_ids.end()) {
		is_found_imp_campagin_flag = true;
		printf("found campaign imp flag\n");
		if (countc.size() == 0) {
			printf("init flag countc!\n");
			InitCountFlag(adInfo->campaign_id, countc, "countc", conn);
		} else {
			printf("update flag countc!\n");
			UpdateFlag(countc);
		}
		ret = CheckFlag(countc, adInfo->campaign_id);
		SetFlag(countc, infos.res, "countc");
		DeleteImpFlag(adInfo->campaign_id, infos.res, "impc");
	} 

	if (impb_ids.find(adInfo->banner_id) != impb_ids.end()) {
		is_found_imp_banner_flag = true;
		printf("found banner imp flag\n");
		if (countb.size() == 0) {
			printf("init flag countb!\n");
			InitCountFlag(adInfo->banner_id, countb, "countb", conn);
		} else {
			printf("update flag countb!\n");
			UpdateFlag(countb);
		}
		ret = CheckFlag(countb, adInfo->banner_id);
		SetFlag(countb, infos.res, "countb");
		DeleteImpFlag(adInfo->banner_id, infos.res, "impb");
	} 

	return ret;
}
            