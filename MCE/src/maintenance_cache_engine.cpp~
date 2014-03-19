#include "maintenance_cache_engine.h"

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include <Models/AdInfo.h>
#include <core/Space.h>
#include <core/Table.h>

#include "sql.h.inc"

#include "mysql_context.h"

NS_USING_DRAGON

CacheProcess::CacheProcess()
{

}

CacheProcess::~CacheProcess()
{

}

void CacheProcess::SetPath()
{
	path_ = MCE_APP_PATH;
}

void CacheTask::Run(dragon::Cycle &c)
{
	std::cout << "Task Run at " << c.path << std::endl;

	MySQLContext sqlCtx;
	sql::Connection *conn = sqlCtx.Connect("127.0.0.1", "root", "", "dap");
	sql::PreparedStatement *stmt = conn->prepareStatement(strQueryAllZone);
	sql::ResultSet *rs = stmt->executeQuery();

	while (rs->next()) {
		std::cout << "zone_id:" << rs->getString("zone_id") << std::endl;
	}

	NamedSemiSpace space("DE_CACHE_DATA", 1024);
	space.Create();

	OffsetTable table(100, space);
	
	AdInfo ad;
	ad.id = 1;
	ad.name = "ajs";

	table.Put(ad.id, space.GetPos());
	ad.Dump(space);

	delete rs;
	delete stmt;
	space.Close();
}

int main() {
	CacheTask t;
	CacheProcess p;
	p.Init();
	p.PushTask(&t);
	p.Run();
	p.Dispose();
	return 0;
}
