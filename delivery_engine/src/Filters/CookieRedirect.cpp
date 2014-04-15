#include <set>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <utility/DateTime.h>
#include <utility/Url.h>
#include <utility/Bz2.h>
#include <utility/StringAlogrithms.h>
#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <core/QueryString.h>


#include "CookieRedirect.h"

using namespace dragon;

CookieRedirector::CookieRedirector(HttpRequest  &request,
				  				   HttpResponse &response, 
				   				   QueryString &qs) :  req_(request), res_(response), qs_(qs)
{

}

void CookieRedirector::readCookie(int type)
{
	std::string cookieName;
	switch(type) 
	{
		case COOKIE_LOG_ZONE:
		cookieName = DAP_LG_Z;
		break;

		case COOKIE_LOG_BANNER:
		cookieName = DAP_LG_B;
		break;

		case COOKIE_CLK_ZONE:
		cookieName = DAP_CK_Z;
		break;

		case COOKIE_CLK_BANNER:
		cookieName = DAP_CK_B;
		break;
	}
	
	if (req_.GetCookie().find(cookieName) == req_.GetCookie().end()) {
		return ;
	}

	//std::string data = Bzip2::Decompress(URL::decode(_request._cookies[cookieName]));
	std::string data = req_.GetCookie()[cookieName];
	std::vector<std::string> v = StringAlogrithms::explode(ITEM_SEPARATOR, data);
	BOOST_FOREACH(const std::string &c, v)
	{
		if (c.empty())
			continue;

		std::string::size_type pos = c.find(PROPERTY_SEPARATOR);
		std::string id = std::string(c, 0, pos);
		int item_id = boost::lexical_cast<int>(id);
		std::cout << item_id << std::endl;

		std::string::size_type pos2 = c.find(PROPERTY_SEPARATOR, pos + 1);
		std::string time = std::string(c, pos + 1, pos2 - pos - 1);
		time_t item_time = boost::lexical_cast<time_t>(time);
		std::cout << item_time << std::endl;

		std::string count = std::string(c, pos2 + 1, c.length());
		int item_count = boost::lexical_cast<int>(count);
		std::cout << item_count << std::endl;

		cookie_item item = {item_id, item_time, item_count};
		switch(type) 
		{
			case COOKIE_LOG_ZONE:
			case COOKIE_CLK_ZONE:
			zoneHistory[item_id]   = item;
			break;

			case COOKIE_LOG_BANNER:
			case COOKIE_CLK_BANNER:
			bannerHistory[item_id] = item;
			break;
		}
	}
}

void CookieRedirector::writeCookie(int type)
{
	std::string cookieName;
	History *pHistroy = NULL;
	switch(type) 
	{
		case COOKIE_LOG_ZONE:
		cookieName = DAP_LG_Z;
		pHistroy   = &zoneHistory;
		break;

		case COOKIE_LOG_BANNER:
		cookieName = DAP_LG_B;
		pHistroy   = &bannerHistory;
		break;

		case COOKIE_CLK_ZONE:
		cookieName = DAP_CK_Z;
		pHistroy   = &zoneHistory;
		break;

		case COOKIE_CLK_BANNER:
		cookieName = DAP_CK_B;
		pHistroy   = &bannerHistory;
		break;
	}

	std::string value;
	BOOST_FOREACH(const cookie_item_pair_t &p, *pHistroy)
	{
		value += boost::lexical_cast<std::string>(p.first) + PROPERTY_SEPARATOR;
		time_t time = p.second.time;
		value += boost::lexical_cast<std::string>(time) + PROPERTY_SEPARATOR;
		value += boost::lexical_cast<std::string>(p.second.count);
		value += ITEM_SEPARATOR;
	}
	value = value.substr(0, value.length() - 1);
	res_.setCookie(cookieName, value, DateTime::YearFromNow1(1), "/");
}

void CookieRedirector::addImpHistory(int zoneid, int bannerid)
{
	readCookie(COOKIE_LOG_ZONE);
	readCookie(COOKIE_LOG_BANNER);

	if (zoneHistory.find(zoneid) != zoneHistory.end()) 
	{
		zoneHistory[zoneid].count++;
		//std::cout << "count :" << zoneHistory[id].count << std::endl;
	} else {	
		cookie_item item = {zoneid, time(NULL), 1};
		zoneHistory[zoneid] = item;
	}


	if (bannerHistory.find(bannerid) != bannerHistory.end()) 
	{
		bannerHistory[bannerid].count++;
		//std::cout << "count :" << bannerHistory[id].count << std::endl;
	} else {	
		cookie_item item = {bannerid, time(NULL), 1};
		bannerHistory[bannerid] = item;
	}
}

void CookieRedirector::addClkHistory(int zoneid, int bannerid)
{
	readCookie(COOKIE_CLK_ZONE);
	readCookie(COOKIE_CLK_BANNER);

	if (zoneHistory.find(zoneid) != zoneHistory.end()) 
	{
		zoneHistory[zoneid].count++;
	} else {	
		cookie_item item = {zoneid, time(NULL), 1};
		zoneHistory[zoneid] = item;
	}
	

	if (bannerHistory.find(bannerid) != bannerHistory.end()) 
	{
		bannerHistory[bannerid].count++;
	} else {	
		cookie_item item = {bannerid, time(NULL), 1};
		bannerHistory[bannerid] = item;
	}
}

cookie_item CookieRedirector::getHistory(int type, int id)
{
	std::string cookieName;
	History *pHistroy = NULL;
	switch(type) 
	{
		case COOKIE_LOG_ZONE:
		cookieName = DAP_LG_Z;
		pHistroy   = &zoneHistory;
		break;

		case COOKIE_LOG_BANNER:
		cookieName = DAP_LG_B;
		pHistroy   = &bannerHistory;
		break;

		case COOKIE_CLK_ZONE:
		cookieName = DAP_CK_Z;
		pHistroy   = &zoneHistory;
		break;

		case COOKIE_CLK_BANNER:
		cookieName = DAP_CK_B;
		pHistroy   = &bannerHistory;
		break;
	}

	BOOST_FOREACH(const cookie_item_pair_t &p, *pHistroy)
	{
		if (p.first == id)
		{
			return p.second;
		}
	}

	cookie_item item = {id, 0, 0};
	return item;
}

void CookieRedirector::readCKCookieBanner()
{
	if (req_.GetCookie().find(DAP_CK_B) == req_.GetCookie().end()) {
		return ;
	}

	std::string data = Bzip2::Decompress(URL::decode(req_.GetCookie()[DAP_CK_B]));
	std::vector<std::string> v = StringAlogrithms::explode(ITEM_SEPARATOR, data);
	BOOST_FOREACH(const std::string &c, v)
	{
		std::string::size_type pos = c.find(PROPERTY_SEPARATOR);
		std::string id = std::string(c, 0, pos);
		try {
			bannerList.insert(boost::lexical_cast<int>(id));
		} catch(boost::bad_lexical_cast &) {
			continue;
		}
	}
}

void CookieRedirector::readCKCookieZone()
{
	if (req_.GetCookie().find(DAP_CK_Z) == req_.GetCookie().end()) {
		return ;
	}

	std::string data = Bzip2::Decompress(URL::decode(req_.GetCookie()[DAP_CK_Z]));
	std::vector<std::string> v = StringAlogrithms::explode(ITEM_SEPARATOR, data);

	BOOST_FOREACH(const std::string &c, v)
	{
		std::string::size_type pos = c.find(PROPERTY_SEPARATOR);
		std::string id = std::string(c, 0, pos);
		try {
			zoneList.insert(boost::lexical_cast<int>(id));
		} catch(boost::bad_lexical_cast &) {
			continue;
		}
	}
}

void CookieRedirector::readLGCookieBanner()
{
	if (req_.GetCookie().find(DAP_LG_B) == req_.GetCookie().end()) {
		return ;
	}

	std::string url = URL::decode(req_.GetCookie()[DAP_LG_B]);
	std::string data = Bzip2::Decompress(url);
	std::vector<std::string> v = StringAlogrithms::explode(ITEM_SEPARATOR, data);
	BOOST_FOREACH(const std::string &c, v)
	{
		std::string::size_type pos = c.find(PROPERTY_SEPARATOR);
		std::string id = std::string(c, 0, pos);
		std::string time = std::string(c, pos + 1, c.length());
		try {
			bannerList.insert(boost::lexical_cast<int>(id));
		} catch(boost::bad_lexical_cast &) {
			continue;
		}
	}
}

void CookieRedirector::readLGCookieZone()
{
	if (req_.GetCookie().find(DAP_LG_Z) == req_.GetCookie().end()) {
		return ;
	}

	std::string url = URL::decode(req_.GetCookie()[DAP_LG_Z]);
	std::string data = Bzip2::Decompress(url);
	std::vector<std::string> v = StringAlogrithms::explode(ITEM_SEPARATOR, data);
	BOOST_FOREACH(const std::string &c, v)
	{
		std::string::size_type pos = c.find(PROPERTY_SEPARATOR);
		std::string id =  std::string(c, 0, pos);
		std::string time = std::string(c, pos + 1, c.length());

		try {
			zoneList.insert(boost::lexical_cast<int>(id));
		} catch(boost::bad_lexical_cast &) {
			continue;
		}
	}
}

void CookieRedirector::readCKCookie(int zone_id, int banner_id)
{
	readCKCookieBanner();
	readCKCookieZone();

	if (banner_id >= 0) {
		bannerList.insert(banner_id);
	}

	if (zone_id >= 0) {
		zoneList.insert(zone_id);
	}
}

void CookieRedirector::readLGCookie(int zone_id, int banner_id)
{
	readLGCookieBanner();
	readLGCookieZone();

	if (banner_id >= 0) {
		bannerList.insert(banner_id);
	}

	if (zone_id >= 0) {
		zoneList.insert(zone_id);
	}
}

void CookieRedirector::writeLGCookie()
{
	std::string value;
	BOOST_FOREACH(int id, bannerList)
	{
		value += boost::lexical_cast<std::string>(id) + PROPERTY_SEPARATOR;
		value += boost::lexical_cast<std::string>(time(NULL));
		value += ITEM_SEPARATOR;
	}
	res_.setCookie(DAP_LG_B, URL::encode(Bzip2::Compress(value)), DateTime::YearFromNow1(1),"/");

	value.clear();
	BOOST_FOREACH(int id, zoneList)
	{
		value += boost::lexical_cast<std::string>(id) + PROPERTY_SEPARATOR;
		value += boost::lexical_cast<std::string>(time(NULL));
		value += ITEM_SEPARATOR;
	}
	res_.setCookie(DAP_LG_Z, URL::encode(Bzip2::Compress(value)), DateTime::YearFromNow1(1),"/");
}

void CookieRedirector::writeCKCookie()
{
	std::string value;
	BOOST_FOREACH(int id, bannerList)
	{
		value += boost::lexical_cast<std::string>(id) + PROPERTY_SEPARATOR;
		value += boost::lexical_cast<std::string>(time(NULL));
		value += ITEM_SEPARATOR;
	}
	res_.setCookie(DAP_CK_B, URL::encode(Bzip2::Compress(value)), DateTime::YearFromNow1(1),"/");

	value.clear();
	BOOST_FOREACH(int id, zoneList)
	{
		value += boost::lexical_cast<std::string>(id) + PROPERTY_SEPARATOR;
		value += boost::lexical_cast<std::string>(time(NULL));
		value += ITEM_SEPARATOR;
	}
	res_.setCookie(DAP_CK_Z, URL::encode(Bzip2::Compress(value)), DateTime::YearFromNow1(1),"/");
}
