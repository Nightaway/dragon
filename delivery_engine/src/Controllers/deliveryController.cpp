#include "deliveryController.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "../Utility/DataModel.h"

NS_USING_DRAGON
NS_USING_BOOST

IMPLEMENT_CONTROLLER(Controller, deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION(deliveryController, ad)
	ACTION_OP1(deliveryController, lg, "__")
	ACTION(deliveryController, ck)
END_ACTION_MAP()

void deliveryController::ad(QueryString &qs)
{
  int zoneid = -1;
  try {
    zoneid = lexical_cast<int>(qs["zoneid"]);
  } catch (bad_lexical_cast &e) {
    response->StringResult(e.what());
    return ;
  }
  printf("zoneid:%d\n", zoneid);

  NamedSemiSpace space("DE_CACHE_DATA", 1024 * 1024);
  space.Open();

  ZoneInfo *zoneInfo = GetZoneInfoById(zoneid, space);
  if (zoneInfo == NULL) {
    std::string strError = "No ZoneInfo found by ";
    strError += boost::lexical_cast<std::string>(zoneid);
    response->StringResult(strError); 
    return ;
  }

  printf("zone id:%d, name:%s\n", zoneInfo->id, zoneInfo->name.c_str());
  BOOST_FOREACH(int bannerid, zoneInfo->linked_banners)
  {
    printf("linked banner id:%d\n", bannerid);
    AdInfo *adInfo = GetAdInfoById(bannerid, space);
    adInfo->Stuff(space);
    printf("ad id:%d, instance:%s\n", adInfo->banner_id, adInfo->template_string.c_str());
  }

  std::string out = "ad action : index=";
  out += qs["index"];
  response->StringResult(out);

  space.Close();
}

void deliveryController::lg(QueryString &qs)
{
  response->StringResult("lg action");
}

void deliveryController::ck(QueryString &qs)
{
  response->StringResult("ck action");
}
