#include "AdInfo.h"

NS_USING_DRAGON

void AdInfo::Dump(Space &space)
{
  space.Put(banner_id);
  space.Put(placement_id);
  space.Put(status);
  space.Put(width);
  space.Put(ext_banner_type);
  space.Put(height);
  space.Put(type);
  space.Put(contenttype);
  space.Put(weight);
  space.Put(adserver);
  space.Put(block_ad);
  space.Put(cap_ad);
  space.Put(session_cap_ad);
  space.Put(compiledlimition);
  space.Put(acl_plugin);
  space.Put(alt_filename);
  space.Put(priority);
  space.Put(priority_factor);
  space.Put(to_be_delivered);
  space.Put(campaign_id);
  space.Put(campaign_priority);
  space.Put(campaign_companion);
  space.Put(block_campaign);
  space.Put(cap_campaign);
  space.Put(session_cap_campaign);
  space.Put(show_capped_no_cookie);
  space.Put(client_id);
  space.Put(expire_time);
  space.Put(revenue_type);
  space.Put(ecpm_enabled);
  space.Put(ecpm);
  space.Put(tracker_status);
  space.Put(ord_id);
  space.Put(template_string);
}

void AdInfo::Stuff(Space &space)
{
  space.Get(banner_id);
  space.Get(placement_id);
  space.Get(status);
  space.Get(width);
  space.Get(ext_banner_type);
  space.Get(height);
  space.Get(type);
  space.Get(contenttype);
  space.Get(weight);
  space.Get(adserver);
  space.Get(block_ad);
  space.Get(cap_ad);
  space.Get(session_cap_ad);
  space.Get(compiledlimition);
  space.Get(acl_plugin);
  space.Get(alt_filename);
  space.Get(priority);
  space.Get(priority_factor);
  space.Get(to_be_delivered);
  space.Get(campaign_id);
  space.Get(campaign_priority);
  space.Get(campaign_companion);
  space.Get(block_campaign);
  space.Get(cap_campaign);
  space.Get(session_cap_campaign);
  space.Get(show_capped_no_cookie);
  space.Get(client_id);
  space.Get(expire_time);
  space.Get(revenue_type);
  space.Get(ecpm_enabled);
  space.Get(ecpm);
  space.Get(tracker_status);
  space.Get(ord_id);
  space.Get(template_string);
}

unsigned AdInfo::Size()
{
  unsigned size_ = 0;
  size_ += sizeof(banner_id);	
  size_ += sizeof(placement_id);
  size_ += sizeof(status);
  size_ += sizeof(width);
  size_ += sizeOf(ext_banner_type);
  size_ += sizeof(height);
  size_ += sizeOf(type);
  size_ += sizeOf(contenttype);
  size_ += sizeof(weight);
  size_ += sizeOf(adserver);
  size_ += sizeof(block_ad);
  size_ += sizeof(cap_ad);
  size_ += sizeof(session_cap_ad);
  size_ += sizeOf(compiledlimition);
  size_ += sizeOf(acl_plugin);
  size_ += sizeOf(alt_filename);
  size_ += sizeof(priority);
  size_ += sizeof(priority_factor);
  size_ += sizeof(to_be_delivered);
  size_ += sizeof(campaign_id);
  size_ += sizeof(campaign_priority);
  size_ += sizeof(campaign_weight);
  size_ += sizeof(campaign_companion);
  size_ += sizeof(block_campaign);
  size_ += sizeof(cap_campaign);
  size_ += sizeof(session_cap_campaign);
  size_ += sizeof(client_id);
  size_ += sizeOf(expire_time);
  size_ += sizeof(revenue_type);
  size_ += sizeof(ecpm_enabled);
  size_ += sizeof(ecpm);
  size_ += sizeof(tracker_status);
  size_ += sizeof(ord_id);
  size_ += sizeOf(template_string);
  return size_;
}

void AdInfo::Stuff(sql::ResultSet *rs)
{
    banner_id    = rs->getInt("ad_id");
    placement_id = rs->getInt("placement_id");
    status       = rs->getInt("status");
    width        = rs->getInt("width");
    ext_banner_type = rs->getString("ext_bannertype");
    height       = rs->getInt("height");
    type         = rs->getString("type");
    contenttype  = rs->getString("contenttype");
    weight       = rs->getInt("weight");
    adserver     = rs->getString("adserver");
    block_ad     = rs->getInt("block_ad");
    cap_ad       = rs->getInt("cap_ad");
    session_cap_ad = rs->getInt("session_cap_ad");
    compiledlimition = rs->getString("compiledlimitation");
    acl_plugin   = rs->getString("acl_plugins");
    alt_filename = rs->getString("alt_filename");
    priority     = rs->getDouble("priority");
    priority_factor = rs->getDouble("priority_factor");
    to_be_delivered = rs->getInt("to_be_delivered");
    campaign_id     = rs->getInt("campaign_id");
    campaign_priority = rs->getInt("campaign_priority");
    campaign_weight = rs->getInt("campaign_weight");
    campaign_companion = rs->getInt("campaign_companion");
    block_campaign  = rs->getInt("block_campaign");
    cap_campaign    = rs->getInt("cap_campaign");
    session_cap_campaign = rs->getInt("session_cap_campaign");
    show_capped_no_cookie = rs->getInt("show_capped_no_cookie");
    client_id       = rs->getInt("client_id");
    expire_time     = rs->getString("expire_time");
    revenue_type    = rs->getInt("revenue_type");
    ecpm_enabled    = rs->getInt("ecpm_enabled");
    ecpm            = rs->getInt("ecpm");
    tracker_status  = rs->getInt("tracker_status");
    ord_id          = rs->getInt("order_id");
}
