#ifndef __ZONEINFO_H__
#define __ZONEINFO_H__

#include <core/Space.h>
#include <core/Object.h>
#include <core/StringRef.h>
#include <core/String.h>
#include <string>

class ZoneInfo : public dragon::Object {
public:
	unsigned Size();
	void Dump(dragon::Space &space);
	void Stuff(dragon::Space &space);

public:
		int	    id;
		std::string name;
		int         type;
		std::string description;
		int         width;
		int         height;
        	std::string chain;
		std::string prepend;
		std::string append;
		int         appendtype;
		std::string forceappend;
		int         inventory_forecast_type;
		int         block_zone;
		int         cap_zone;
		int         session_cap_zone;
		int         show_capped_no_cookie_zone;
		std::string ext_adselection;
		int         publisher_id;
		int         agency_id;
		int         trafficker_account_id;
		int         manager_account_id;
		std::string ad_render;
		std::string ad_render_name;
		std::string ad_render_value_true;
		std::string ad_render_value_false;
};

#endif
