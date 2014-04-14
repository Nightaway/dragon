#ifndef __COOKIE_REDIRECT_H__
#define __COOKIE_REDIRECT_H__

#include <map>
#include <set>
#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <core/QueryString.h>

#define DAP_LG_Z "RDTIMPZ"
#define DAP_LG_B "RDTIMPB"
#define DAP_CK_Z "RDTCLKZ"
#define DAP_CK_B "RDTCLKB"

#define PROPERTY_SEPARATOR "."
#define ITEM_SEPARATOR "_"
	
enum {
	COOKIE_LOG_ZONE = 0,
	COOKIE_LOG_BANNER,
	COOKIE_CLK_ZONE,
	COOKIE_CLK_BANNER
};

struct cookie_item {
	int id;
	time_t time;
	int count;
};

typedef std::set<int> BannerList;
typedef std::set<int> ZoneList;
typedef std::map<int, cookie_item> History;
typedef std::pair<int, cookie_item> cookie_item_pair_t;

class CookieRedirector {
	dragon::HttpRequest  &req_;
	dragon::HttpResponse &res_;
	dragon::QueryString &qs_;

	BannerList bannerList;
	ZoneList   zoneList;

	History bannerHistory;
	History zoneHistory;

public:
	CookieRedirector(dragon::HttpRequest  &request,
				     dragon::HttpResponse &response, 
				     dragon::QueryString &qs);

	void readCookie(int type);
	void writeCookie(int type);

	void addImpHistory(int zoneid, int bannerid);
	void addClkHistory(int zoneid, int bannerid);
	cookie_item  getHistory(int type, int id);
	
	void readCKCookieBanner();
	void readCKCookieZone();
	void readLGCookieBanner();
	void readLGCookieZone();

	void readLGCookie(int zone_id, int banner_id);
	void readCKCookie(int zone_id, int banner_id);

	void writeLGCookie();
	void writeCKCookie();

	BannerList &getBannerList()
	{
		return bannerList;
	}

	ZoneList &getZoneList()
	{
		return zoneList;
	}

	History &getBannerHistory()
	{
		return bannerHistory;
	}

	History &getZoneHistory()
	{
		return zoneHistory;
	}
};

#endif