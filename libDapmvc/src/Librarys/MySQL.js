var q = "SELECT " +
   " d.bannerid AS bannerid, " +
   " d.campaignid AS placement_id, " +
   " d.status AS status, " +
   " d.width AS width, " +
   " d.ext_bannertype AS ext_bannertype, " +
   " d.height AS height, " +
   " d.storagetype AS type, " +
   " d.contenttype AS contenttype, " +
   " d.weight AS weight, " +
   " d.adserver AS adserver, " +
   " d.block AS block_ad, " +
   " d.capping AS cap_ad, " +
   " d.session_capping AS session_cap_ad, " +
   " d.compiledlimitation AS compiledlimitation, " +
   " d.acl_plugins AS acl_plugins, " +
   " d.alt_filename AS alt_filename, " +
   " az.priority AS priority, " +
   " az.priority_factor AS priority_factor, " +
   " az.to_be_delivered AS to_be_delivered, " +
   " c.campaignid AS campaign_id, " +
   " c.priority AS campaign_priority, " +
   " c.weight AS campaign_weight, " +
   " c.companion AS campaign_companion, " +
   " c.block AS block_campaign, " +
   " c.capping AS cap_campaign, " +
   " c.session_capping AS session_cap_campaign, " +
   " c.show_capped_no_cookie AS show_capped_no_cookie, " +
   " c.clientid AS client_id, " +
   " c.expire_time AS expire_time, " +
   " c.revenue_type AS revenue_type, " +
   " c.ecpm_enabled AS ecpm_enabled, " +
   " c.ecpm AS ecpm, " +
   " ct.status AS tracker_status " +
"FROM" +
   " `dap_banners` AS d " +
       " JOIN " +
    " `dap_ad_zone_assoc` AS az ON (d.bannerid = az.ad_id) " +
      "  JOIN " +
    " `dap_campaigns` AS c ON (c.campaignid = d.campaignid) " +
       " LEFT JOIN " +
    " `dap_campaigns_trackers` AS ct ON (ct.campaignid = c.campaignid) " +
"WHERE" +
    " az.zone_id = ? AND d.status <= 0 " +
       "AND c.status <= 0 ;";

var sqlQueryInstance = "SELECT * FROM dap_banner_views WHERE banner_id = ?;";

function AdInfo(id, weight, priority, priority_factor) {
    this.id = id;
    this.weight = weight;
    this.priority = priority;
    this.priority_factor = priority_factor;
}

function Campaign(id) {
    this.id = id;
}

function WeightCampaign(id, weight) {
    Campaign.call(this, id);
    this.weight = weight;
    this.ads      = new Array();
}

function ZoneLinkedAds(id) {
    this.id = id;
    this.exCamps = new Array();
}

WeightCampaign.prototype = Object.create(Campaign.prototype);

exports.queryInstanceByBannerid = function(bannerid) {
    var ctx = Library.Load('SqlContext');
    var conn = ctx.Connect('127.0.0.1', 'root', '', 'dap');
    var stmt  = conn.PrepStmt(sqlQueryInstance);
    stmt.Set(1, bannerid);
    var rs      =  stmt.ExecuteQuery();
    stmt.Dispose();

    var instance;
    if (rs.Next()) {
        instance  = rs.GetString(3);
    }
    rs.Dispose();
    return instance;
}


function findCampaign(id, camps)
{
            for (var i=0; i<camps.length; ++i)
            {
                    if (id == camps[i].id) 
                     {
                          return camps[i];
                     } 
            }
            return null;
}

exports.queryAdsByZoneid = function(zoneid) {
    var ctx = Library.Load('SqlContext');
    var conn = ctx.Connect('127.0.0.1', 'root', '', 'dap');
    var stmt  = conn.PrepStmt(q);
    stmt.Set(1, zoneid);
    var rs = stmt.ExecuteQuery();
    stmt.Dispose();

    var ads = new ZoneLinkedAds(zoneid);
    while (rs.Next()) {
        if (rs.Get('campaign_priority') == -1) {
           var ad = new AdInfo(rs.Get('bannerid'), rs.Get('weight'),  rs.Get('priority'), rs.Get('priority_factor'));
           var camp = findCampaign(rs.Get('placement_id'), ads.exCamps);
            if (camp) {
                     camp.ads.push(ad);
            } else {
                      var ex = new WeightCampaign(rs.Get('placement_id'), rs.Get('campaign_weight'));
                      ex.ads.push(ad);
                      ads.exCamps.push(ex);
            }
       
        }
    }
    rs.Dispose();
    return ads;
}

exports.queryAds = function() {
    var ctx = Library.Load('SqlContext');

    var host = '127.0.0.1';
    var user = 'root';
    var pass = '';
    var db = 'dap';

    var SqlConn = ctx.Connect(host, user, pass, db);
    // SqlConn.SetScheme('dap');
    
    var PrepStmt = SqlConn.PrepStmt(q);
    PrepStmt.Set(1, 5549);
    var rs = PrepStmt.ExecuteQuery();
    PrepStmt.Dispose();

    var banners = [];
    var idx = 0;

    while (rs.Next()) {
        banners[idx++] = {
            'bannerid' : rs.Get('bannerid'),
            'imageurl' : rs.Get('imageurl'),
        };
    }
    banners.count = idx;
    rs.Dispose();

    return banners;
};