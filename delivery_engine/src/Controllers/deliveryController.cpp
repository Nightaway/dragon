#include "deliveryController.h"

#include <utility/DateTime.h>
#include <core/View.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../config.h"
#include "../Utility/DataModel.h"
#include "../Utility/Select.h"
#include "../Filters/Filter.h"

NS_USING_DRAGON
NS_USING_BOOST

IMPLEMENT_CONTROLLER(Controller, deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION(deliveryController, ad)
	ACTION(deliveryController, lg)
	ACTION_OP1(deliveryController, ck, "__")
	ACTION(deliveryController, test)
END_ACTION_MAP()

extern unsigned char gif_buf[43];

void deliveryController::test(QueryString &qs)
{
  NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
  space.Open();
  bool isHeadUsed = space.IsHeadUsed();

  response->StringResult("test " + boost::lexical_cast<std::string>(isHeadUsed));
  space.Close();
}

void deliveryController::ad(QueryString &qs)
{
  Log &error = app->GetErrorLog();
  int zoneid = 0;
  try {
    zoneid = lexical_cast<int>(qs["zoneid"]);
  } catch (bad_lexical_cast &e) {
    error.LogFmt("zoneid [%s] %s", qs["zoneid"].c_str(), e.what());
    response->StringResult(" ");
    return;
  }

  NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
  space.Open();

  boost::scoped_ptr<ZoneInfo> zoneInfo(GetZoneInfoById(zoneid, space));
  if (zoneInfo == NULL) {
    error.LogFmt("ZoneInfo by id [%d], not found in Shared Memory Object named [%s] size[%d]", 
                  zoneid, SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
    response->StringResult(" "); 
    space.Close();
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

    printf("ad id:%d, weight:%d,instance:%s\n", adInfo->banner_id, adInfo->campaign_weight,adInfo->template_string.c_str());
    
    adInfos.push_back(adInfo);
  }

  Information info = {
    request,
    response,
    &qs,
  };
  filter(adInfos, info, VisitorFilter);
  filter(adInfos, info, StandardFilter);

  AdInfo *adInfo = SelectOne(adInfos);

  if (adInfo == NULL) {
    std::string out = "no ad selected";
    response->StringResult(out);
    space.Close();
    return ;
  }

  int rnd = std::rand();
  View v("__db");
  v.DataBind(qs);
  v.DataBind(*config);

  ViewBag &bag = v.getViewBag();
  bag["rnd"] = boost::lexical_cast<std::string>(rnd);
  bag["cb"] =   bag["rnd"];

  ViewEngine ve;
  std::string result = ve.Out(v, adInfo->template_string);
  response->StringResult(result);
  space.Close();
  return ;
}

void deliveryController::lg(QueryString &qs)
{
  Log &error = app->GetErrorLog();
  int zoneid = 0, bannerId = 0, campaignid = 0;

  try {
    zoneid = boost::lexical_cast<int>(qs["zoneid"]);
    bannerId = boost::lexical_cast<int>(qs["bannerid"]);
    if (!qs["campaignid"].empty())
      campaignid = boost::lexical_cast<int>(qs["campaignid"]);
  } catch (boost::bad_lexical_cast) {
    error.LogFmt("lg action cast error: zoneid [%s], bannerid [%s], campaignid [%s]", 
          qs["zoneid"].c_str(), qs["bannerid"].c_str(), qs["campaignid"].c_str());
    response->DataRefResult(DataRef(gif_buf, 43));
    return ;
  }

  if (!qs["campaignid"].empty()) {
    std::string impc = "impc[" +  qs["campaignid"] + "]";
    response->setCookie(impc, "1", DateTime::YearFromNow(1), "/");
  }

  std::string impb = "impb[" +  qs["bannerid"] + "]";
  response->setCookie(impb, "1", DateTime::YearFromNow(1), "/");

  response->DataRefResult(DataRef(gif_buf, 43));
  return ;
}

void deliveryController::ck(QueryString &qs)
{
  response->StringResult("ck action");
}

unsigned char gif_buf[43] = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x01, 0x00, 
			      0x01, 0x00, 0x80, 0x00, 0x00, 0xff, 0xff, 0xff, 
			      0x00, 0x00, 0x00, 0x21, 0xf9, 0x04, 0x00, 0x00, 
			      0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 
			      0x01, 0x00, 0x01, 0x00, 0x00, 0x02, 0x02, 0x44, 
			      0x01, 0x00, 0x3b };
