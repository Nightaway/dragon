#include "deliveryController.h"

NS_USING_DRAGON

IMPLEMENT_CONTROLLER(Controller, deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION(deliveryController, ad)
	ACTION_OP1(deliveryController, lg, "__")
	ACTION(deliveryController, ck)
END_ACTION_MAP()

void deliveryController::ad(QueryString &qs)
{
	std::string out = "ad action : index=";
	out += qs["index"];
	response->StringResult(out);
}

void deliveryController::lg(QueryString &qs)
{
	response->StringResult("lg action");
}

void deliveryController::ck(QueryString &qs)
{
	response->StringResult("ck action");
}
