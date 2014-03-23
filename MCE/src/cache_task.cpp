#include "cache_task.h"

#include <iostream>
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

#include "sql.h.inc"

#include "mysql_context.h"

NS_USING_DRAGON

std::string Now()
{
  	char buf[30];
	time_t now = time(NULL);
	tm *newtime = localtime(&now);
	strftime(buf, 30, "%Y-%m-%d %H:%M:%S", newtime);
        return std::string(buf);
}

void CacheTask::Run(dragon::Cycle &c)
{
	std::cout << "Task Run at " << c.path << std::endl;

	std::vector<int> zoneids;
	MySQLContext sqlCtx;
	sql::Connection *conn = sqlCtx.Connect(c.config["mysql"]["host"].c_str(), c.config["mysql"]["user"].c_str(), c.config["mysql"]["pass"].c_str(), c.config["mysql"]["db"].c_str());

	sql::PreparedStatement *stmt = conn->prepareStatement(strQueryAllZone);
	sql::ResultSet *rs = stmt->executeQuery();
	while (rs->next()) {
		std::cout << "zone_id:" << rs->getString("zone_id") << std::endl;
		zoneids.push_back(rs->getInt("zone_id"));
	}
	delete rs;
	delete stmt;

	NamedSemiSpace space("DE_CACHE_DATA", 1024 * 1024);
	space.Create();
	OffsetTable table(1000, space);
        OffsetTable adTable(1000, space);

	BOOST_FOREACH(int zoneid, zoneids)
	{
		sql::PreparedStatement *stmt = conn->prepareStatement(strQueryZoneInfo);
		stmt->setInt(1, zoneid);
		sql::ResultSet *rs = stmt->executeQuery();
		ZoneInfo zoneInfo;
		zoneInfo.Stuff(rs);
		std::cout << "zone name:" << zoneInfo.name << std::endl;

		table.Put(zoneid, space.GetPos());
		zoneInfo.Dump(space);

                std::string now = Now();
                sql::PreparedStatement *stmtAds = conn->prepareStatement(strQueryZoneLinkedAds);
                stmtAds->setInt(1, zoneid);
     	        stmtAds->setString(2, now);
            	stmtAds->setString(3, now);
                sql::ResultSet *rsAds = stmtAds->executeQuery();

                while (rsAds->next()) {
                  AdInfo ad;
                  ad.Stuff(rsAds);
                  std::cout << "banner id:" << ad.banner_id << std::endl;
                }

                delete rsAds;
                delete stmtAds;
		delete rs;
		delete stmt;
        }

	space.Close();
}
