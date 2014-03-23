#ifndef __ADINFO_H__
#define __ADINFO_H__

#include <core/Space.h>
#include <core/Object.h>
#include <core/StringRef.h>
#include <core/String.h>
#include <string>
#include <cppconn/resultset.h>

class AdInfo : public dragon::Object {
public:
	unsigned Size();
	void Dump(dragon::Space &space);
	void Stuff(dragon::Space &space);
        void Stuff(sql::ResultSet *rs);

public:
	int banner_id;
        int placement_id;
        int status;
	int width;
	std::string ext_banner_type;
	int height;
	std::string type;
	std::string contenttype;
	int weight;
	std::string adserver;
	int block_ad;
	int cap_ad;
	int session_cap_ad;
        std::string compiledlimition;
	std::string acl_plugin;
	std::string alt_filename;
	double priority;
	double priority_factor;
	int to_be_delivered; 	
	int campaign_id;
	int campaign_priority;
	int campaign_weight;
        int campaign_companion;
        int block_campaign;
	int cap_campaign;
	int session_cap_campaign;
	int show_capped_no_cookie;
	int client_id;
        std::string expire_time;
	int revenue_type;
	int ecpm_enabled;
	int ecpm;
	int tracker_status;
	int ord_id;
        std::string template_string;
};

#endif
