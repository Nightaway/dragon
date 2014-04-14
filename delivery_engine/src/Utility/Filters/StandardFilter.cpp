#include "../Filter.h"

#include "../../config.h"
#include "../IPLocator.h"
#include "../libRedis/redisclient.h"
#include "../libRedis/tests/functions.h"
#include "../RedisContext.h"

#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <core/Macro.h>
#include <core/QueryString.h>
#include <utility/DateTime.h>
#include <utility/Url.h>
#include <utility/StringAlogrithms.h>

#include "RegexInfo.h"
#include "CookieRedirect.h"
#include "MotuFilter.h"
#include "ContentFilter.h"

NS_USING_DRAGON

struct StandardInfo {
	std::string ip_data;
	std::string browser;
	std::string pageurl;
	std::string geo_country;
	std::string geo_city;
	std::string accept_language;
	std::string os;
	std::string hostname;
	std::string userAgent;
	QueryString  *qs;
	HttpResponse *res;
	HttpRequest  *req;
	Config       *config;
};

bool MAX_checkMotuFilter_motuFilter(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSearchTagFilter_searchTagFilter(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSearchFilter_searchFilter(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkCookieRedirect_ImpZone(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkCookieRedirect_ImpBanner(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkCookieRedirect_ClkZone(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkCookieRedirect_ClkBanner(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkGeo_Country(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkGeo_Region(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkGeo_City(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSite_Referingpage(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSite_Source(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSite_Variable(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Useragent(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Domain(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Os(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Language(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkTime_Date(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkTime_Day(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkTime_Hour(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Browser(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkClient_Ip(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkSite_Pageurl(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkCookieThreshold_cookieThreshold(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkQueryParameters_queryParameters(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkPageContext_pageContext(std::string limition, std::string op, StandardInfo *hyl_user_info);
bool MAX_checkVideoContext_videoContext(std::string limition, std::string op, StandardInfo *hyl_user_info);

bool MAX_checkClient_Ip_eq(std::string limition,std::string op,std::string str_ip);
bool MAX_limitationsMatchStringValue(std::string value,std::string limition,std::string op);
bool MAX_limitationMatchNumeric(std::string key,std::string value,std::string op, QueryString &request);
bool MAX_limitationsMatchString(std::string key,std::string value,std::string op,QueryString &request);
bool MAX_limitationsMatchArrayValue(std::string limition,std::string op,std::string value);
bool pcadLimitationsCookieRedirectDeliveryMatch(std::set<int> &params, std::string &limition, std::string &op);
bool PaizeQuanjingFilterCoreseekQuery(std::string &limition, std::string &searches, std::string &op, std::string &type, Config &config);
std::string getVideoContent(std::string &content);

typedef struct {
	std::string func_name;
	bool (*func)(std::string, std::string, StandardInfo *);
} func_map;
typedef boost::function<int (std::string, std::string, StandardInfo *)> check_func_t;
typedef boost::unordered_map<std::string, check_func_t> check_func_map_t;

static func_map check_func_map[] = {
						{"MAX_checkClient_Browser", MAX_checkClient_Browser},
						{"MAX_checkClient_Ip", MAX_checkClient_Ip}, 
						{"MAX_checkSite_Pageurl", MAX_checkSite_Pageurl},
						{"MAX_checkGeo_Region", MAX_checkGeo_Region},
						{"MAX_checkGeo_City", MAX_checkGeo_City},
						{"MAX_checkGeo_Country" , MAX_checkGeo_Country},
						{"MAX_checkTime_Hour", MAX_checkTime_Hour},
						{"MAX_checkTime_Day", MAX_checkTime_Day},
						{"MAX_checkTime_Date", MAX_checkTime_Date},
						{"MAX_checkClient_Language", MAX_checkClient_Language},
						{"MAX_checkClient_Os", MAX_checkClient_Os},
						{"MAX_checkClient_Domain", MAX_checkClient_Domain},
						{"MAX_checkClient_Useragent", MAX_checkClient_Useragent},
						{"MAX_checkSite_Variable", MAX_checkSite_Variable},
						{"MAX_checkSite_Referingpage", MAX_checkSite_Referingpage},
						{"MAX_checkSite_Source", MAX_checkSite_Source},
						{"MAX_checkCookieThreshold_cookieThreshold", MAX_checkCookieThreshold_cookieThreshold},
						{"MAX_checkMotuFilter_motuFilter", MAX_checkMotuFilter_motuFilter},
						{"MAX_checkCookieRedirect_ImpZone", MAX_checkCookieRedirect_ImpZone},
						{"MAX_checkCookieRedirect_ImpBanner", MAX_checkCookieRedirect_ImpBanner},
						{"MAX_checkCookieRedirect_ClkZone", MAX_checkCookieRedirect_ClkZone},
						{"MAX_checkCookieRedirect_ClkBanner", MAX_checkCookieRedirect_ClkBanner},
						{"MAX_checkQueryParameters_queryParameters", MAX_checkQueryParameters_queryParameters},
						{"MAX_checkPageContext_pageContext", MAX_checkPageContext_pageContext},
						{"MAX_checkVideoContext_videoContext", MAX_checkVideoContext_videoContext},
				};

size_t func_map_len = sizeof(check_func_map) / sizeof(check_func_map[0]);
check_func_map_t check_map;

static IPLocator ipLocator;
RedisContext redisContext;

//
static std::string BrowserLongNames[29] ={ "microsoft internet explorer","msie","netscape6","netscape","galeon","phoenix","mozilla firebird","firebird","firefox","chimera","camino","epiphany","safari","k-meleon","mozilla","opera","konqueror","icab","lynx","links","ncsa mosaic","amaya","omniweb","hotjava","browsex","amigavoyager","amiga-aweb","ibrowse","chrome"};
static std::string BrowserShortNames[29]={"IE","IE","NS","NS","GA","PX","FB","FB","FX","CH","CA","EP","SF","KM","MZ","OP","KQ","IC","LX","LI","MO","AM","OW","HJ","BX","AV","AW","IB","GC"};
static boost::unordered_map<std::string, std::string> BrowserMap;

//
static std::string CookieLongNames[12]={"blockAd","capAd","sessionCapAd","blockCampaign","capCampaign","sessionCapCampaign","blockZone","capZone","sessionCapZone","lastView","lastClick","blockLoggingClick"};
static std::string CookieShortNames[12] = {"OABLOCK","OACAP","OASCAP","OACBLOCK","OACCAP","OASCCAP","OAZBLOCK","OAZCAP","OASZCAP","OXLIA","OXLCA","OXBLC"};
static boost::unordered_map<std::string, std::string> CookieMap;

static void BuildStandardInfo(StandardInfo &standardInfo, const Information &infos);
static void BuildOSInfo(const std::string &userAgent, StandardInfo &standardInfo);
static void BuildBrowserInfo(const std::string &userAgent, StandardInfo &standardInfo);
static void BuildCountryAndCityInfo(const std::string &ip, StandardInfo &standardInfo);

bool StandardFilter(AdInfo *adInfo, Information &infos)
{
	StandardInfo standardInfo;
	BuildStandardInfo(standardInfo, infos);
	return true;
}

bool StandardFilterInit()
{
	//	浏览器对照表初始化
	for(int i=0; i < 29; i++)
	{
		BrowserMap[BrowserLongNames[i]] = BrowserShortNames[i];
	}

	//	限制型cookie对照表初始化
	for(int i=0; i < 12; i++)
	{
		CookieMap[CookieLongNames[i]]= CookieShortNames[i];
	}

	// 加载IP库
	ipLocator.open(DE_APP_PATH "data/ipLib.data");

	// check function map
	//int len = sizeof(check_func_map) /  sizeof(check_func_map[0]);
	for (int i=0; i<func_map_len; ++i)
	{
		std::string key = check_func_map[i].func_name;
		check_func_t func = check_func_map[i].func;
		check_map[key] = func;
	}
}

static void BuildStandardInfo(StandardInfo &standardInfo, const Information &infos)
{
	standardInfo.res = infos.res;
	standardInfo.req = infos.req;
	standardInfo.qs  = infos.qs;
	standardInfo.config = infos.config;

	QueryString &qs  = *standardInfo.qs;
	HttpRequest &req = *standardInfo.req;

	std::string location = qs["loc"];
	if (!location.empty())	{
		standardInfo.pageurl = boost::replace_all_copy(location, ":", "%3A");
	}

	std::string userAgent = req.GetUserAgent().string();
	if (!userAgent.empty())
	{
		standardInfo.userAgent = userAgent;
		std::string userAgent = boost::to_lower_copy(userAgent);

		BuildOSInfo(userAgent, standardInfo);
		BuildBrowserInfo(userAgent, standardInfo);
	}

	//	得到语言信息和IP信息
	standardInfo.accept_language = req.GetAcceptLanguage().string();
	standardInfo.ip_data		 = req.GetIp().string();

	if (!standardInfo.ip_data.empty())
	{
		BuildCountryAndCityInfo(standardInfo.ip_data, standardInfo);
	}

	// 根据ip地址解析网站域名
	//_limitInfo.hostname = _limitInfo.ip_data;

	// hostname
	//BuildHostInfo(_limitInfo.ip_data);

	// 获取 parameters
	//_limitInfo.params = _params;
}

void BuildCountryAndCityInfo(const std::string &ip, StandardInfo &standardInfo)
{
	const int MAX_LENGTH = 60;
	char area[MAX_LENGTH] = "UNKOWN";
	char country[MAX_LENGTH] = "UNKOWN";

	unsigned int uip = ipLocator.ip_str2int(ip.c_str());
	ipLocator.getIpAddress(uip, country, area);
	standardInfo.geo_country = country;
	standardInfo.geo_city    = area;
}

void BuildBrowserInfo(const std::string &userAgent, StandardInfo &standardInfo)
{
		//	得到用户浏览器
		std::vector<boost::sregex_token_iterator> vec;
		boost::sregex_token_iterator i(userAgent.begin(), userAgent.end(), browser_regex);
		boost::sregex_token_iterator j;
		while (i != j)
		{
			vec.push_back(i++);
		}

		//	一般情况下user_agent满足正则表达的最后一组匹配项为用户所使用的浏览器，在这里还加了一个额外因素 
		if ((vec.size() > 1)&&(*(vec[vec.size()-1]) == "safari") && (*(vec[vec.size()-2]) == "chrome"))	{
			standardInfo.browser = "GC";		
		} else	{
			if (vec.size() != 0)
				standardInfo.browser = BrowserMap[ *(vec[vec.size() - 1])];
		}
}

void BuildOSInfo(const std::string &userAgent, StandardInfo &standardInfo)
{
    boost::smatch result;

	//	 Windows OS Infomation
	std::string os;
   if(regex_search(userAgent, result, regex_windows))
	{	
		if((result[2].str() == "nt") && (result[3].str() == "5.1"))
		{
			os="xp";
		}
   		else if ((result[2].str() == "nt") && (result[3].str() == "6.1")) 
		{
			os="w7";
		}
		else if (result[2].str() == "2000") 
		{
			os="2k";
		}
		else if ((result[2].str() == "nt") && (result[3].str() == "5.0"))
		{
			os="2k";
		}
		else if ((result[2].str() == "9x")&&(result[3].str() == "4.9")) 
		{
			os="98";
		}
		else
		{
			os= result[2].str() + result[3].str();
		}
			
		if(result[2] == '\0')
			os="win";
		standardInfo.os = os;
   }	
   else if(regex_search(userAgent, result, regex_amiga)) 	
	{	
		//	amiga OS Infomatioon
		boost::regex regex_amiga1("(AmigaOS [\\.1-9]?)");
		if(userAgent.find("morphos",0 ) != std::string::npos)	
		{
			os = "morphos";
		}
		else if(userAgent.find("mc680x0",0) != std::string::npos)	
		{
			os = "mc680x0";
		}
		else if(userAgent.find("ppc",0) != std::string::npos)
		{
			os = "ppc";
		}
		else if(regex_search(userAgent, result, regex_amiga1))
		{
			os = std::string(result[1]);
		}
		standardInfo.os = os;
	}
	else if(regex_search(userAgent, result, regex_os2))
	{
		// os2
		standardInfo.os = "os2";
	}
	else if(regex_search(userAgent, result, regex_mac))
	{
		// mac
		if(result[1].matched)
			os="68k";
		else 
			os="";
		if(result[2].matched) 
			os="osx";
		if(result[3].matched) 
			os="ppc";
		if(result[4].matched) 
			os="osx";

		standardInfo.os = os;
	}
	else if(regex_search(userAgent, result, regex_sunos))
	{
		// os = sun|sun4|sun5|suni86
		if (userAgent.find("sun",0) != std::string::npos) 
				os="sun"+std::string(result[1]);
		standardInfo.os = os+ result[2].str();
	}		
	else if(regex_search(userAgent, result, regex_irix))
	{
		// os = irix|irix5|irix6|
		standardInfo.os = result[1].str() + result[2].str();
	}		
	else if(regex_search(userAgent, result, regex_hpux))
	{
		// os = hpux9|hpux10
		os = std::string(result[1]);
		std::string::size_type search1;
		search1=os.find_first_of("-");
		os = os.replace(search1, 1, "");
		standardInfo.os = os + result[2].str();
	}	
	else if(regex_search(userAgent, result, regex_aix))
	{
		//  os = aix|aix1|aix2|aix3|
		standardInfo.os = "aix" + result[1].str();
	}	
	else if(regex_search(userAgent, result, regex_dec))
	{
		/// os = dec
		standardInfo.os = "dec";
	}		
	else if(regex_search(userAgent, result, regex_vms))
	{
		standardInfo.os = "vms";
	}	
	else if(regex_search(userAgent, result, regex_sco))
	{
		// os = sco
		standardInfo.os = "sco";
	}	
	else if(userAgent.find("unix_system_v",0) != std::string::npos)
	{
		standardInfo.os = "unixware";
	}
	else if(userAgent.find("ncr",0) != std::string::npos)
	{
		// os = mpras
		standardInfo.os = "mpras";
	}

/**********************reliant sets: platform = *nix ; os = reliant*************************************************************/
	
	else if(userAgent.find("reliantunix",0) != std::string::npos)
	{
		standardInfo.os = "reliant";
//		os_plat1.platform="*nix";
	}

/**********************sinix sets: platform = *nix ; os = sinix*************************************************************/
	
	else if(userAgent.find("sinix",0) != std::string::npos)
	{
		standardInfo.os = "sinix";
//		os_plat1.platform="*nix";
	}

/**********************bsd sets: platform = *nix ; os = bsd|freebsd*************************************************************/
	
	else if(regex_search(userAgent, result, regex_bsd))
	{
		standardInfo.os = result[1] + result[2];
//		os_plat1.platform="*nix";
	}	

/**********************linux sets: platform = *nix ; os = linux*************************************************************/
	
	else if(regex_search(userAgent, result, regex_linux))
	{
		standardInfo.os = "linux";
//		os_plat1.platform="*nix";
	}	
}


bool MAX_checkSite_Referingpage(std::string limition,std::string op, StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	std::string referer = (*hyl_user_info->qs)["referer"];
	if(referer.empty())
		return false;
	referer = URL::decode(referer);
	return MAX_limitationsMatchStringValue(referer, limition, op);
}

bool MAX_checkSite_Source(std::string limition,std::string op,StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	std::string source = (*hyl_user_info->qs)["source"];
	if(source.empty())
		return false;
	source = URL::decode(source);
	return MAX_limitationsMatchStringValue(source, limition, op);
}

bool MAX_checkSite_Variable(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string key,value;
	size_t index = 0; 
	index = limition.find("|", 0);
	key = limition.substr(0,index);
	value = limition.substr(index+1, limition.size());

	if (limition.empty() || (*hyl_user_info->qs)[key].empty())
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));
	else if((op == "gt") || (op == "lt")) {
		return MAX_limitationMatchNumeric(key, value, op, *hyl_user_info->qs);
    } else {
		return MAX_limitationsMatchString(key, value, op, *hyl_user_info->qs);
    }
}

bool MAX_checkClient_Useragent(std::string limition,std::string op,StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	std::string useragent = hyl_user_info->userAgent;

	return MAX_limitationsMatchStringValue(useragent, limition, op);
}

bool MAX_checkClient_Domain(std::string limition,std::string op,StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	std::string domain;
	if(op == "!x" || op=="=x" )
		domain = hyl_user_info->hostname;
	else
	{
		std::string host = hyl_user_info->hostname;
		int start = host.length()-limition.length();
		if (start < 0) start = 0;
		domain = host.substr(start,host.length());
	}
	domain = URL::decode(domain);
	return MAX_limitationsMatchStringValue(domain, limition, op);
}

bool MAX_checkClient_Os(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string os = hyl_user_info->os;
	if (limition.empty()||os.empty())
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));

	return MAX_limitationsMatchArrayValue(limition,op,os);
}

bool MAX_checkClient_Language(std::string limition, 
			      			  std::string op, 
			      			  StandardInfo *hyl_user_info)
{
	std::string language = hyl_user_info->accept_language;
	if (language.empty() || limition.empty())
		return true;

	std::vector<std::string> langs;
	std::vector<std::string> limits;

	langs = dragon::StringAlogrithms::explode(",", language);
	limits = dragon::StringAlogrithms::explode(",", limition);

	std::string lang_temp, limit_temp;
	bool mark = false;

	for(unsigned int i = 0; i < langs.size(); i++)
	{
		lang_temp = langs[i];
		if (mark == 1)
			break;

		for(unsigned int j = 0; j < limits.size(); j++)
		{
			limit_temp = limits[j];
			if (lang_temp.find(limit_temp,0) != std::string::npos)
			{
				mark = 1;
				break;
			}
		}
	}

	if((op == "=~") && (mark == 1)) return true;
	else if((op == "=~") && (mark != 1)) return false;
	else if((op == "!~") && (mark == 1)) return false;
	else if((op == "!~") && (mark != 1)) return true;

	return true;
}

bool MAX_checkTime_Day(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	size_t index = 0;
	
	index = limition.find("@",0);
	if (index != std::string::npos)
	{
		limition = limition.substr(0,index);
	}
	else
		return true;

	if (limition.empty())
		return true;
	else
	{
		struct tm *local;
	    time_t t;
		t=time(NULL);
		local=localtime(&t);   
		std::string value = boost::lexical_cast<std::string>(local->tm_wday);

		return MAX_limitationsMatchArrayValue(limition,op,value);
	}
}

bool MAX_checkTime_Date(std::string limition,std::string op,StandardInfo *hyl_user_info)
{
	size_t index = 0;
	
	index = limition.find("@",0);
	if (index != std::string::npos)
	{
		limition = limition.substr(0,index);
	}
	else
		return true;

	if (limition.empty() || limition == "00000000")
		return true;
	else {
		struct tm *local;
		char tmpbuf[10];
   		time_t t = time(NULL);
		local=localtime(&t);
		std::string value;
		strftime(tmpbuf, 10, "%Y%m%d", local);
		value = tmpbuf;

		int limit_num = atoi(limition.c_str());
		int value_num = atoi(value.c_str());

		if (op == "==") return (value_num == limit_num);
		else if (op == "!=") return (value_num != limit_num);
		else if (op == "<=") return (value_num <= limit_num);
		else if (op == ">=") return (value_num >= limit_num);
		else if (op == "<")  return (value_num < limit_num);
		else if (op == ">")  return (value_num > limit_num);
	
		return true;
	}
}

bool MAX_checkTime_Hour(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	size_t index = 0;
	
	index = limition.find("@",0);
	if (index != std::string::npos)
	{
		limition = limition.substr(0,index);
	}
	else
		return true;

	if (limition.empty())
		return true;
	else
	{
		struct tm *local;
	    time_t t;
		t=time(NULL);
		local=localtime(&t);   
		std::string value = boost::lexical_cast<std::string>(local->tm_hour);

		return MAX_limitationsMatchArrayValue(limition,op,value);
	}
}

bool MAX_checkGeo_Region(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string temp;
	std::string region = hyl_user_info->geo_country;
	if (op != "=~" && op != "!~") {
        // Provide backwards compatibility
        op = "=~";
    }

	/*	一般设置 有可能为 cn|22,30,09 先去掉“｜”号，再以“，”为分隔符存入mulit_region中*/
	size_t index = limition.find("|");

	if(index == limition.length() -1)	
	{
		temp = limition.substr(0,index);
		if(region == "other")
			region = hyl_user_info->geo_city;
		else
			region = "cn";
	}else	if (index != std::string::npos)
		temp = limition.substr(index+1,limition.size());
	else	
		temp = limition;

	return MAX_limitationsMatchArrayValue(temp,op,region);
}

bool MAX_checkGeo_City(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string temp;
	std::string city = hyl_user_info->geo_city;
	if (op != "=~" && op != "!~") {
        // Provide backwards compatibility
        op = "=~";
    }
	/*	一般设置 有可能为 cn|beijing,guangzhou先去掉“｜”号，再以“，”为分隔符存入multi_city中*/
	size_t index = limition.find("|",0);


	if(index == (limition.length()-1))
	{		
		temp = limition.substr(0,index);			
		if(hyl_user_info->geo_country != "other")
			city = "cn";
	}
	else if (index != std::string::npos)
		temp = limition.substr(index+1,limition.size());
	else 
		temp = limition;
	return MAX_limitationsMatchArrayValue(temp, op, city);
}

bool MAX_checkGeo_Country(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string region = hyl_user_info->geo_country;
	std::string city = hyl_user_info->geo_city;
	if(region != "other")
		city = "cn";
	return MAX_limitationsMatchArrayValue(boost::to_lower_copy(limition), op, city);	

}

bool MAX_checkClient_Browser(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string browser = hyl_user_info->browser;
	if (limition.empty()||browser.empty())
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));
	return MAX_limitationsMatchArrayValue(limition,op,browser);
}

bool MAX_checkClient_Ip(std::string limition,std::string op,StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	bool mark = 0;
	limition = boost::trim_copy(limition);
	std::string str_ip = hyl_user_info->ip_data;
	size_t index = 0;
	std::string temp;
	index = limition.find("," ,0);

	while(index != std::string::npos)
	{
		temp = limition.substr(0, index);
		mark = MAX_checkClient_Ip_eq(temp, op, str_ip);
		if (mark == 0) return mark;
		limition = limition.substr(index+1,limition.size());
		index = limition.find(",",0);
	}

	if (index == std::string::npos)
		mark = MAX_checkClient_Ip_eq(limition, op, str_ip);
	return mark;
}

bool MAX_checkClient_Ip_eq(std::string limition, std::string op, std::string str_ip)
{	
	int net[4];
	int mask[4];
	int ip[4];
	size_t index;
	boost::smatch result;
	std::string net1,mask1;
	std::string temp;

	/* 	查看限制条件中是否存在"/" 	*/
	index=limition.find("/",0);
	if(index == std::string::npos)
	{
		  if (boost::regex_match(limition, result, ip_regex))
		  {
			  for(int i=1;i<5;i++)
			  {

			  if(result[i]=="*")
				{
					net[i-1] = 0;
					mask[i-1]= 0;
				}
			  else 
				{
					mask[i-1] = 255;
					temp=result[i];
					net[i-1]  = atoi(temp.c_str());
				}
			  }
		  }
	}
	else
	{
		net1=limition.substr(0,index);
		mask1=limition.substr(index+1,limition.size());		
		
		 if (boost::regex_match(net1, result, ip_regex))
			{				
				temp=result[1];
				net[0]=atoi(temp.c_str());
				temp=result[2];
				net[1]=atoi(temp.c_str());
				temp=result[3];
				net[2]=atoi(temp.c_str());
				temp=result[4];
				net[3]=atoi(temp.c_str());	
		 }

		  if (boost::regex_match(mask1, result, ip_regex))
			{				
				temp=result[1];
				mask[0]=atoi(temp.c_str());
				temp=result[2];
				mask[1]=atoi(temp.c_str());
				temp=result[3];
				mask[2]=atoi(temp.c_str());
				temp=result[4];
				mask[3]=atoi(temp.c_str());	
		 }		
	}

	 if (boost::regex_match(str_ip, result, ip_regex))
			{				
				temp=result[1];
				ip[0]=atoi(temp.c_str());
				temp=result[2];
				ip[1]=atoi(temp.c_str());
				temp=result[3];
				ip[2]=atoi(temp.c_str());
				temp=result[4];
				ip[3]=atoi(temp.c_str());	
		 }

	bool mark1 = 1;
	for(int i=0;i<4;i++)
	{
		ip[i]&=mask[i];
		if (ip[i]!=net[i]) {mark1=0;break;}
	}

	 bool mark2 = 0;
	if (op=="==") mark2=1;
	
	return (mark1==mark2);	
}

bool MAX_checkSite_Pageurl(std::string limition, std::string op, StandardInfo *hyl_user_info)
{	
	if (limition.empty())
		return true;

	std ::string pageurl = hyl_user_info->pageurl;
	pageurl = URL::decode(pageurl);
	return MAX_limitationsMatchStringValue(pageurl, limition, op);
}

bool MAX_limitationsMatchArrayValue(std::string limition, std::string op, std::string value)
{
	std::string split = ",";
	std::string split_value = split + value + split;
	std::string split_limition = split + limition + split;
	if (op == "==")
		return (split_limition == split_value);
	else if (op == "=~"){
		if (split_limition.empty())
			return true;
		return (split_limition.find(split_value, 0) != std::string::npos);
	}
	else {
		if (split_value.empty())
			return false;
		return (split_limition.find(split_value,0) == std::string::npos);
	}
}

bool MAX_limitationMatchNumeric(std::string key, std::string value, std::string op, QueryString &request)
{
	std::string request_value;
	if (value.empty())
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));
	if ((request[key].find_first_not_of("0123456789") == std::string::npos)||(value.find_first_not_of("0123456789") == std::string::npos))
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));
	else
		request_value = request[key];

	if (op == "lt")
		return (atoi(request_value.c_str()) < atoi(value.c_str()));
	else if (op == "gt")
		return (atoi(request_value.c_str()) > atoi(value.c_str()));
	else
		return !((op == "==") || (op == "=~") || (op == "=x") || (op == "gt") || (op == "lt"));
}

bool MAX_limitationsMatchString(std::string key, std::string value,std::string op, QueryString &request)
{
	if (value.empty())
		return true;
	std::string request_value = request[key];
	return MAX_limitationsMatchStringValue(request_value, value, op);
}

bool MAX_limitationsMatchStringValue(std::string value, std::string limition, std::string op)
{
	value = boost::to_lower_copy(value);
	limition = boost::to_lower_copy(limition);

	if (op=="==")
		return (limition == value);
	else if (op=="!=")
		return (limition != value);
	else if (op=="=~")
		return (value.find(limition,0) != std::string::npos);
	else if (op=="!~")
		return (value.find(limition,0) == std::string::npos);
	else if (op=="=x")
	{
		std::string split = "#";
		limition = boost::replace_all_copy(limition, "#", "\\#");
		boost::regex value_regex;
		value_regex = limition;	
		boost::smatch result;		
		return boost::regex_search(value, result, value_regex)?1:0;		
	}
	else
	{
		std::string split = "#";
		limition =  boost::replace_all_copy(limition, "#", "\\#");
		boost::regex value_regex;
		value_regex = limition;	
		boost::smatch result;		
		return boost::regex_search(value, result, value_regex)?0:1;		
	}	
	return true;
}

bool MAX_checkCookieThreshold_cookieThreshold_matchIsNotAnyOf(std::vector<std::string> &,redis::client::string_pair_vector &, HttpResponse &);
bool MAX_checkCookieThreshold_cookieThreshold_matchIsAnyOf(std::vector<std::string> &,redis::client::string_pair_vector &, HttpResponse &);
bool MAX_checkCookieThreshold_cookieThreshold(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if (limition.empty())
		return true;

	const std::string PCID = hyl_user_info->req->GetCookie()["PCID"];
	boost::shared_ptr<redis::client> c_ptr(redisContext.Connect(PCID));
	redis::client & c = *c_ptr;

	redis::client::string_pair_vector entries;
	c.hgetall(PCID, entries);

	int mark = false;
	std::vector<std::string> limits = dragon::StringAlogrithms::explode(",", limition);

	if (op == "!~")
		mark = MAX_checkCookieThreshold_cookieThreshold_matchIsNotAnyOf(limits, entries, *hyl_user_info->res);
	else if (op == "=~")
		mark = MAX_checkCookieThreshold_cookieThreshold_matchIsAnyOf(limits, entries, *hyl_user_info->res);

	return mark;
}

bool MAX_checkCookieThreshold_cookieThreshold_matchIsNotAnyOf(std::vector<std::string> &limit, redis::client::string_pair_vector &entries, HttpResponse &response)
{
	int mark = true;
	for(unsigned int ix = 0; ix != limit.size();ix++)
	{
		std::vector<std::string> limitThres = dragon::StringAlogrithms::explode("|", limit[ix]);

		int mark1 = 0;
		unsigned int i = 0;
		for(; i<entries.size(); ++i) 
		{
			if (entries[i].first ==  limitThres[1])
				{mark1 = 1;break;}
		}
		if (mark1 != 1)
			continue;
		
		int bitThres = atoi(entries[i].second.c_str());
		int temp_limit2 = atoi(limitThres[2].c_str());
		int temp_limit3 = atoi(limitThres[3].c_str());
		if(temp_limit2 >= 100 || bitThres >= temp_limit3)
		{
				mark = false;
				std::string temp = limitThres[1] + ":" + entries[i].second;
				response.setCookie("TARGET_BIT", temp, DateTime::YearFromNow(1), "/");
				break;
		}
	}
	return mark;
}

bool MAX_checkCookieThreshold_cookieThreshold_matchIsAnyOf(std::vector<std::string> &limit, redis::client::string_pair_vector &entries, HttpResponse &response)
{
	int mark = false;
	for(unsigned int ix = 0; ix != limit.size();ix++)
	{
		std::vector<std::string> limitThres = dragon::StringAlogrithms::explode("|", limit[ix]);

		bool mark1 = false;
		unsigned int i = 0;
		for(; i<entries.size(); ++i) 
		{
			if (entries[i].first ==  limitThres[1])
			{
				mark1 = true;
				break;
			}
		}
		if (mark1 != true)
			continue;
		
		int bitThres = atoi(entries[i].second.c_str());
		int temp_limit2 = atoi(limitThres[2].c_str());
		int temp_limit3 = atoi(limitThres[3].c_str());
		if(temp_limit2 >= 100 || bitThres >= temp_limit3)
		{
			mark = true;
			std::string temp = limitThres[1] + ":" + entries[i].second;
			response.setCookie("TARGET_BIT", temp, DateTime::YearFromNow(1), "/");
			break;
		}
	}
	return mark;
}

bool MAX_checkCookieRedirect_ImpZone(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty()){
		return true;
	}

	std::vector<std::string> limits1;
	std::vector<std::string> limits2;
	std::set<int> zonelimit;
	limits1 = StringAlogrithms::explode("|", limition);
	limits2 = StringAlogrithms::explode(",", limits1[1]);
	BOOST_FOREACH(const std::string  &e, limits2)
	{
		try {
					zonelimit.insert(boost::lexical_cast<int>(e));
				} catch (boost::bad_lexical_cast)
				{
					return false;
				}		
	}
	CookieRedirector cr(*hyl_user_info->req, *hyl_user_info->res, *hyl_user_info->qs);
	cr.readLGCookieZone();
	ZoneList &zoneParam = cr.getZoneList();
	std::set<int> intersection;
	std::set<int>::iterator iter = intersection.begin() ;
	std::set_intersection(zoneParam.begin() , zoneParam.end() , zonelimit.begin(), zonelimit.end() , inserter(intersection,iter));
	
	if(op == "=~")
	{
		return zoneParam.empty()?false : (intersection.size()>0?true:false);
	}
	else
	{
		return zoneParam.empty()?true : (intersection.size()>0?false:true);
	}
}

bool MAX_checkCookieRedirect_ImpBanner(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty()){
		return true;
	}

	std::vector<std::string> limits1;
	std::vector<std::string> limits2;
	std::set<int> bannerlimit;
	limits1 = StringAlogrithms::explode("|", limition);
	limits2 = StringAlogrithms::explode(",", limits1[1]);
	BOOST_FOREACH(const std::string  &e, limits2)
	{
		try {
					bannerlimit.insert(boost::lexical_cast<int>(e));
				} catch (boost::bad_lexical_cast)
				{
					return false;
				}		
	}
	CookieRedirector cr(*hyl_user_info->req, *hyl_user_info->res, *hyl_user_info->qs);
	cr.readLGCookieBanner();
	BannerList &BannerParam = cr.getBannerList();
	std::set<int> intersection;
	std::set<int>::iterator iter = intersection.begin() ;
	std::set_intersection(BannerParam.begin() , BannerParam.end() , bannerlimit.begin(), bannerlimit.end() , inserter(intersection,iter));
	
	if(op == "=~")
	{
		return BannerParam.empty()?false : (intersection.size()>0?true:false);
	}
	else
	{
		return BannerParam.empty()?true : (intersection.size()>0?false:true);
	}
}

bool MAX_checkCookieRedirect_ClkZone(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty()){
		return true;
	}

	std::vector<std::string> limits1;
	std::vector<std::string> limits2;
	std::set<int> zonelimit;
	limits1 = StringAlogrithms::explode("|", limition);
	limits2 = StringAlogrithms::explode(",", limits1[1]);
	BOOST_FOREACH(const std::string  &e, limits2)
	{
		try {
					zonelimit.insert(boost::lexical_cast<int>(e));
				} catch (boost::bad_lexical_cast)
				{
					return false;
				}		
	}
	CookieRedirector cr(*hyl_user_info->req, *hyl_user_info->res, *hyl_user_info->qs);
	cr.readCKCookieZone();
	ZoneList &zoneParam = cr.getZoneList();
	std::set<int> intersection;
	std::set<int>::iterator iter = intersection.begin() ;
	std::set_intersection(zoneParam.begin() , zoneParam.end() , zonelimit.begin(), zonelimit.end() , inserter(intersection,iter));
	
	if(op == "=~")
	{
		return zoneParam.empty()?false : (intersection.size()>0?true:false);
	}
	else
	{
		return zoneParam.empty()?true : (intersection.size()>0?false:true);
	}
}

bool MAX_checkCookieRedirect_ClkBanner(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty()){
		return true;
	}

	std::vector<std::string> limits1;
	std::vector<std::string> limits2;
	std::set<int> bannerlimit;
	limits1 = StringAlogrithms::explode("|", limition);
	limits2 = StringAlogrithms::explode(",", limits1[1]);
	BOOST_FOREACH(const std::string  &e, limits2)
	{
		try {
					bannerlimit.insert(boost::lexical_cast<int>(e));
				} catch (boost::bad_lexical_cast)
				{
					return false;
				}		
	}
	CookieRedirector cr(*hyl_user_info->req, *hyl_user_info->res, *hyl_user_info->qs);
	cr.readCKCookieBanner();
	BannerList &BannerParam = cr.getBannerList();
	std::set<int> intersection;
	std::set<int>::iterator iter = intersection.begin() ;
	std::set_intersection(BannerParam.begin() , BannerParam.end() , bannerlimit.begin(), bannerlimit.end() , inserter(intersection,iter));
	
	if(op == "=~")
	{
		return BannerParam.empty()?false : (intersection.size()>0?true:false);
	}
	else
	{
		return BannerParam.empty()?true : (intersection.size()>0?false:true);
	}
}



bool MAX_checkMotuFilter_motuFilter(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	std::string car_id;
	std::string temp = (*hyl_user_info->qs)["pcvars"];
	if(!temp.empty())
		car_id = temp.substr(9,temp.length());
	MotuFilter motufilter;
	return motufilter.motu_filter(limition, op, car_id);
}

bool MAX_checkQueryParameters_queryParameters(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty())
		return true;

	std::string param;
	if(!(*hyl_user_info->qs)["pcvars"].empty())
	{
		param = URL::decode((*hyl_user_info->qs)["pcvars"]);
	}
	if(param.empty())
		return true;

	std::vector<std::string> params;
	params = StringAlogrithms::explode("&", param);
	std::map<std::string , std::string > parameters;
	BOOST_FOREACH(const std::string  &e, params)
	{
		std::vector<std::string> p;
		p =  StringAlogrithms::explode("=", e);
		if(!p[1].empty())
			parameters[p[0]] =URL::decode(p[1]);
		else
			parameters[p[0]] = "0";
	}

	std::vector<std::string> alimition;
	alimition = StringAlogrithms::explode("=>", limition);

	bool match = false;

	std::map<std::string , std::string>::iterator iter = parameters.begin();
	for(;iter !=  parameters.end(); iter++)
	{
		int left = 0, right = 0;
		if(iter->first == alimition[0])
		{
			try{
				left = boost::lexical_cast<int>(boost::trim_copy(iter->second));
			}catch (boost::bad_lexical_cast)
			{
				return false;
			}

			try{
				right = boost::lexical_cast<int>(boost::trim_copy(alimition[1]));
			}catch (boost::bad_lexical_cast)
			{
				return false;
			}

			if(op ==  "==")
			{
				match = ( left == right);
			}else if(op ==  "!=")
			{
				match = ( left != right);
			}else if(op ==  ">")
			{
				match = ( left > right);
			}else if(op ==  "<")
			{
				match = ( left < right);
			}else if(op ==  ">=")
			{
				match = ( left >= right);
			}else if(op ==  "<=")
			{
				match = ( left<= right);
			}
		}
	
	}
	return match;
}

bool MAX_checkPageContext_pageContext(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty())
		return true;

	if(((*hyl_user_info->qs)["type"] != "mmt") && ((*hyl_user_info->qs)["type"] != "ajs")&&((*hyl_user_info->qs)["type"] != "rpls"))
		return true;

	if((*hyl_user_info->qs)["loc"].empty())
		return true;

	std::string context = getContent(URL::decode((*hyl_user_info->qs)["loc"]));

	std::vector<std::string> limitations = StringAlogrithms::explode(",", limition);
    
   bool matchEqual = (op == "=~" ? true : false);
    /*
     * 默认返回值
     * 当 op == '=~' 时，默认返回 false
     * 否则，默认返回 true
     */
    bool match = matchEqual ? false : true;
    
    /*
     * 定向设置的关键词，逐个与广告页面关键词匹配
     * 当定向逻辑为“包含”时，只要有一个定向关键词，与页面关键词匹配，则返回 true
     * 当定向逻辑为“不包含”时，只要有一个定向关键词，与页面关键词匹配，则返回 false
     */
	boost::to_upper(context);
    BOOST_FOREACH (const std::string &keyword, limitations)
    {
		std::string keywords = boost::to_upper_copy(keyword);
		if (context.find(keywords) != std::string::npos)
        {
            return matchEqual ? true : false;
        }
    }
    return match;
}

bool MAX_checkVideoContext_videoContext(std::string limition, std::string op, StandardInfo *hyl_user_info)
{
	if(limition.empty()) {
		return true;
	}

	bool match = false;
	const std::string &movieid = (*hyl_user_info->qs)["movieid"];
	limition = "shixian," + limition;

	if(op == "=~") {
		if((*hyl_user_info->qs)["movieid"].empty()) {
			return false;
		}

		if(limition.find(movieid) != std::string::npos)
			match = true;	
	
	} else if (op == "!~") {
		if((*hyl_user_info->qs)["movieid"].empty()) {
			return true;
		}
		if(limition.find(movieid) != std::string::npos)
			match = false;	
	}
	return match;
}