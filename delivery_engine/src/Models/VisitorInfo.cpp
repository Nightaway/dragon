#include "VisitorInfo.h"

#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

const char *VisitorInfo::strQueryVisitorFilterInfoByCId = "SELECT `dap_visitorsfilter`.campaignid as id, `dap_visitorsfilter`.type," "`dap_visitorsfilter`.count, `dap_visitorsfilter`.time, `activate_time`, `expire_time`" 
" FROM " 
" `dap_campaigns` LEFT JOIN `dap_visitorsfilter` "
" ON dap_campaigns.campaignid = `dap_visitorsfilter`.campaignid "
" WHERE `dap_visitorsfilter`.campaignid = ? and `dap_visitorsfilter`.bannersid=0";

const char *VisitorInfo::strQueryVisitorFilterInfoByBId = "SELECT `dap_visitorsfilter`.bannersid as id, `dap_visitorsfilter`.type," "`dap_visitorsfilter`.count, `dap_visitorsfilter`.time, `activate_time`, `expire_time`" 
" FROM " 
" `dap_campaigns` LEFT JOIN `dap_visitorsfilter` "
" ON dap_campaigns.campaignid = `dap_visitorsfilter`.campaignid "
" WHERE `dap_visitorsfilter`.bannersid = ?";

unsigned VisitorInfo::Size()
{
  return 0;
}

void VisitorInfo::Dump(dragon::Space &space)
{

}

void VisitorInfo::Stuff(dragon::Space &space)
{

}

void VisitorInfo::Stuff(sql::Connection *conn, int id, int type)
{
  this->id = id;
  const char *strQuery = NULL;
  if (type == kFilterTypeCampaign) {
       strQuery = strQueryVisitorFilterInfoByCId;
  } else if (type == kFilterTypeBanner) {
       strQuery = strQueryVisitorFilterInfoByBId;
  }

  std::auto_ptr<sql::PreparedStatement> prep_stmt(conn->prepareStatement(strQuery));
  prep_stmt->setInt(1, id);	
  std::auto_ptr<sql::ResultSet> rs(prep_stmt->executeQuery());
  while (rs->next()) {
    Visitor info = {rs->getString("type"), rs->getInt("count"), rs->getInt("time"), rs->getString("activate_time")};
    infos.push_back(info);
  }
}
