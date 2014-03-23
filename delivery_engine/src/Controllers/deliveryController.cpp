#include "deliveryController.h"

#include "../Models/AdInfo.h"
#include "../Models/ZoneInfo.h"

NS_USING_DRAGON

IMPLEMENT_CONTROLLER(Controller, deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION(deliveryController, ad)
	ACTION_OP1(deliveryController, lg, "__")
	ACTION(deliveryController, ck)
END_ACTION_MAP()

void deliveryController::ad(QueryString &qs)
{
	NamedSemiSpace space("DE_CACHE_DATA", 1024 * 1024);
	space.Open();
	OffsetTable table;
	table.Open(space);
	unsigned offset = table.Get(601);
	printf("offset:%d\n", offset);

	space.SetPos(offset);
	//AdInfo ad;
	//ad.Stuff(space);
	//printf("ad id:%d, str:%s\n", ad.id, ad.name.c_str());
	ZoneInfo zoneInfo;
	zoneInfo.Stuff(space);
	printf("zone id:%d, name:%s\n", zoneInfo.id, zoneInfo.name.c_str());

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
