#include "cache_task.h"

#include <string>
#include <vector>

#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <Models/AdInfo.h>
#include <Models/ZoneInfo.h>
#include <core/Space.h>
#include <core/Table.h>
#include <db/MysqlContext.h>

#include "config.h"
#include "sql.h.inc"


NS_USING_DRAGON

std::string Now()
{
  char buf[30];
  time_t now = time(NULL);
  tm *newtime = localtime(&now);
  strftime(buf, 30, "%Y-%m-%d %H:%M:%S", newtime);
  return std::string(buf);
}

void GetAllZoneId(sql::Connection *conn,
                  std::vector<int> &zoneids)
{
  assert(conn);
  boost::scoped_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(strQueryAllZone));
  boost::scoped_ptr<sql::ResultSet> rs(stmt->executeQuery());
  while (rs->next()) {
    zoneids.push_back(rs->getInt("zone_id"));
  }
}

sql::ResultSet *GetBannerResultSet(sql::Connection *conn, 
                                   int zoneid)
{
  assert(conn);
  std::string now = Now();
  boost::scoped_ptr<sql::PreparedStatement> stmtAds(conn->prepareStatement(strQueryZoneLinkedAds));
  stmtAds->setInt(1, zoneid);
  stmtAds->setString(2, now);
  stmtAds->setString(3, now);
  sql::ResultSet *rsAds = stmtAds->executeQuery();
  
  return rsAds;
}


NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
void CacheTask::Run(dragon::Cycle &c)
{
  std::cout << "Task Run at " << c.path << std::endl;

  std::vector<int> zoneids;
  MysqlContext sqlCtx;
  sql::Connection *conn = sqlCtx.Connect(c.config["mysql"]["host"].c_str(), 
                                         c.config["mysql"]["user"].c_str(), 
                                         c.config["mysql"]["pass"].c_str(), 
                                         c.config["mysql"]["db"].c_str());

  GetAllZoneId(conn, zoneids);

  if (!space.IsCreated())
	space.Create();
   else
        space.Open();
  space.Switch();
  OffsetTable zoneTable(1000, space);
  OffsetTable adTable(1000, space);
  BOOST_FOREACH(int zoneid, zoneids)
  {
    boost::scoped_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(strQueryZoneInfo));
    stmt->setInt(1, zoneid);
    boost::scoped_ptr<sql::ResultSet> rs(stmt->executeQuery());
    ZoneInfo zoneInfo;
    zoneInfo.Stuff(rs.get());
    printf("zoneid %d\n", zoneid);

    boost::scoped_ptr<sql::ResultSet> rsBanners(GetBannerResultSet(conn, zoneid));
    while (rsBanners != NULL && rsBanners->next()) {
       AdInfo ad;
       ad.Stuff(rsBanners.get());
       zoneInfo.linked_banners.push_back(ad.banner_id);
    }
       
    zoneTable.Put(zoneid, space.GetPos());
    zoneInfo.Dump(space);
  }

  BOOST_FOREACH(int zoneid, zoneids)
  {
    boost::scoped_ptr<sql::ResultSet> rsAds(GetBannerResultSet(conn, zoneid));
     while (rsAds->next()) {
       AdInfo ad;
       ad.Stuff(rsAds.get());
                  
       boost::scoped_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(strQueryInstance));
       stmt->setInt(1, ad.banner_id);
       boost::scoped_ptr<sql::ResultSet> rsInstance(stmt->executeQuery());

        if (rsInstance->next()) {
           ad.template_string = rsInstance->getString("instance");
        }
                  
        unsigned offset = adTable.Get(ad.banner_id);
        if (offset == 0) {
          printf("banner_id:%d\n", ad.banner_id);
          adTable.Put(ad.banner_id, space.GetPos());
          ad.Dump(space);
        }

     }
     
   }

  space.Close();
}
