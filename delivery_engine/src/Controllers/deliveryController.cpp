#include "deliveryController.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "../config.h"
#include "../Utility/DataModel.h"
#include "../Utility/Filter.h"

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
  Log &error = app->GetErrorLog();
  int zoneid = -1;
  try {
    zoneid = lexical_cast<int>(qs["zoneid"]);
  } catch (bad_lexical_cast &e) {
    error.LogFmt("zoneid [%s] %s", qs["zoneid"].c_str(), e.what());
    response->StringResult("");
    return;
  }
  printf("zoneid:%d\n", zoneid);

  NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
  space.Open();

  ZoneInfo *zoneInfo = GetZoneInfoById(zoneid, space);
  if (zoneInfo == NULL) {
    std::string strError = "ZoneInfo by id [";
    strError += boost::lexical_cast<std::string>(zoneid);
    strError += "] ";
    strError += "not found in Shared Memory Object named [";
    strError += SHARED_MEM_OBJ_NAME "] size [";
    strError += boost::lexical_cast<std::string>(SHARED_MEM_OBJ_SIZE);
    strError += "].";
    error.LogFmt(strError.c_str());
    response->StringResult(""); 
    return ;
  }
  printf("zone id:%d, name:%s\n", zoneInfo->id, zoneInfo->name.c_str());

  std::vector<AdInfo *> adInfos;
  BOOST_FOREACH(int bannerid, zoneInfo->linked_banners)
  {
    printf("linked banner id:%d\n", bannerid);
    AdInfo *adInfo = GetAdInfoById(bannerid, space);
    if (adInfo == NULL)
      continue;

    printf("ad id:%d, instance:%s\n", adInfo->banner_id, adInfo->template_string.c_str());
    
    adInfos.push_back(adInfo);
  }

  Information info = {
    request,
    response
  };
  filter(adInfos, info, CookieFilter);

  BOOST_FOREACH(AdInfo *ad, adInfos)
  {
    printf("ad id:%d\n", ad->banner_id);
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
