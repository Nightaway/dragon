#include "ZoneInfo.h"

#include <boost/foreach.hpp>

NS_USING_DRAGON

void ZoneInfo::Dump(Space &space)
{
	space.Put(id);
	space.Put(name);
	space.Put(type);
	space.Put(description);
	space.Put(width);
	space.Put(height);
	space.Put(chain);
	space.Put(prepend);
	space.Put(append);
	space.Put(forceappend);
	space.Put(inventory_forecast_type);
	space.Put(block_zone);
	space.Put(cap_zone);
	space.Put(session_cap_zone);
	space.Put(show_capped_no_cookie_zone);
	space.Put(ext_adselection);
	space.Put(publisher_id);
	space.Put(agency_id);
	space.Put(trafficker_account_id);
	space.Put(manager_account_id);
	space.Put(ad_render);
	space.Put(ad_render_name);
	space.Put(ad_render_value_true);
	space.Put(ad_render_value_false);

        space.Put(linked_banners.size());
        BOOST_FOREACH(int bannerid, linked_banners)
        {
          space.Put(bannerid);
        }
}

void ZoneInfo::Stuff(Space &space)
{
	space.Get(id);
	space.Get(name);
	space.Get(type);
	space.Get(description);
	space.Get(width);
	space.Get(height);
	space.Get(chain);
	space.Get(prepend);
	space.Get(append);
	space.Get(forceappend);
	space.Get(inventory_forecast_type);
	space.Get(block_zone);
	space.Get(cap_zone);
	space.Get(session_cap_zone);
	space.Get(show_capped_no_cookie_zone);
	space.Get(ext_adselection);
	space.Get(publisher_id);
	space.Get(agency_id);
	space.Get(trafficker_account_id);
	space.Get(manager_account_id);
	space.Get(ad_render);
	space.Get(ad_render_name);
	space.Get(ad_render_value_true);
	space.Get(ad_render_value_false);

        size_t size = 0;
        space.Get(size);
        for (size_t i=0; i<size; ++i)
        {
          int bannerid;
          space.Get(bannerid);
          linked_banners.push_back(bannerid);
        }
}

unsigned ZoneInfo::Size()
{
	unsigned size_ = 0;
	size_ += sizeof(id);	
	size_ += sizeOf(name);
	size_ += sizeof(type);
	size_ += sizeOf(description);
	size_ += sizeof(width);
	size_ += sizeof(height);
	size_ += sizeOf(chain);
	size_ += sizeOf(prepend);
	size_ += sizeof(appendtype);
	size_ += sizeOf(forceappend);
	size_ += sizeof(appendtype);
	size_ += sizeof(inventory_forecast_type);
	size_ += sizeof(block_zone);
	size_ += sizeof(cap_zone);
	size_ += sizeof(session_cap_zone);
	size_ += sizeof(show_capped_no_cookie_zone);
	size_ += sizeOf(ext_adselection);
	size_ += sizeof(publisher_id);
	size_ += sizeof(agency_id);
	size_ += sizeof(trafficker_account_id);
	size_ += sizeof(manager_account_id);
	size_ += sizeOf(ad_render);
	size_ += sizeOf(ad_render_name);
	size_ += sizeOf(ad_render_value_true);
	size_ += sizeOf(ad_render_value_false);
	return size_;
}

void ZoneInfo::Stuff(sql::ResultSet *rs)
{
	if (rs->next()) {
		id   = rs->getInt("id");
		name = rs->getString("name");
		type = rs->getInt("type");
		description = rs->getString("description");
		width = rs->getInt("width");
		height = rs->getInt("height");
		chain = rs->getString("chain");
		prepend = rs->getString("prepend");
		append  = rs->getString("append");
		appendtype = rs->getInt("appendtype");
		forceappend = rs->getString("forceappend");
		inventory_forecast_type = rs->getInt("inventory_forecast_type");
		block_zone = rs->getInt("block_zone");
		cap_zone   = rs->getInt("cap_zone");
		session_cap_zone = rs->getInt("session_cap_zone");
		show_capped_no_cookie_zone = rs->getInt("show_capped_no_cookie_zone");
		ext_adselection = rs->getString("ext_adselection");
		publisher_id = rs->getInt("publisher_id");
		agency_id = rs->getInt("agency_id");
		trafficker_account_id = rs->getInt("trafficker_account_id");
		manager_account_id = rs->getInt("manager_account_id");
		ad_render = rs->getString("ad_render");
		ad_render_name = rs->getString("ad_render_name");
		ad_render_value_true = rs->getString("ad_render_value_true");
		ad_render_value_false = rs->getString("ad_render_value_false");
	}
}
