#ifndef __VISITOR_INFO_H__
#define __VISITOR_INFO_H__

#include <vector>
#include <core/Object.h>
#include <cppconn/connection.h>

enum {
  kFilterTypeCampaign,
  kFilterTypeBanner
};

struct Visitor {
  std::string type;
  int count;
  int time;
  std::string startDate;
};

class VisitorInfo {
 public:
  unsigned Size();
  void Dump(dragon::Space &space);
  void Stuff(dragon::Space &space);
  void Stuff(sql::Connection *conn, int id, int type);
 
private:
  static const char *strQueryVisitorFilterInfoByCId;
  static const char *strQueryVisitorFilterInfoByBId;
  int id;
  std::vector<Visitor> infos;
};


#endif
