#include "../Filter.h"


#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <core/Macro.h>
#include <core/QueryString.h>

#include "RegexInfo.h"

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
	QueryString *qs;
	HttpResponse *res;
	HttpRequest *req;
};

//
static std::string BrowserLongNames[29] ={ "microsoft internet explorer","msie","netscape6","netscape","galeon","phoenix","mozilla firebird","firebird","firefox","chimera","camino","epiphany","safari","k-meleon","mozilla","opera","konqueror","icab","lynx","links","ncsa mosaic","amaya","omniweb","hotjava","browsex","amigavoyager","amiga-aweb","ibrowse","chrome"};
static std::string BrowserShortNames[29]={"IE","IE","NS","NS","GA","PX","FB","FB","FX","CH","CA","EP","SF","KM","MZ","OP","KQ","IC","LX","LI","MO","AM","OW","HJ","BX","AV","AW","IB","GC"};
static boost::unordered_map<std::string, std::string> BrowserMap;

static void BuildStandardInfo(StandardInfo &standardInfo, const Information &infos);
static void BuildOSInfo(const std::string &userAgent, StandardInfo &standardInfo);
static void BuildBrowserInfo(const std::string &userAgent, StandardInfo &standardInfo);

bool StandardFilter(AdInfo *adInfo, Information &infos)
{
	StandardInfo standardInfo;
	BuildStandardInfo(standardInfo, infos);
	return true;
}

static void BuildStandardInfo(StandardInfo &standardInfo, const Information &infos)
{
	standardInfo.res = infos.res;
	standardInfo.req = infos.req;
	standardInfo.qs  = infos.qs;

	QueryString &qs  = *standardInfo.qs;
	HttpRequest &req = *standardInfo.req;

	std::string location = qs["loc"];
	if (!location.empty())
	{
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

