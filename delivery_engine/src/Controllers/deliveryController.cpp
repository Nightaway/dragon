#include "deliveryController.h"

#include <utility/DateTime.h>
#include <core/View.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../config.h"

NS_USING_DRAGON
NS_USING_BOOST

IMPLEMENT_CONTROLLER(Controller, deliveryController)

BEGIN_ACTION_MAP(deliveryController)
	ACTION_OP1(deliveryController, ck, "__")
	ACTION(deliveryController, test)
END_ACTION_MAP()

void deliveryController::test(QueryString &qs)
{
  NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
  space.Open();
  bool isHeadUsed = space.IsHeadUsed();

  response->StringResult("test " + boost::lexical_cast<std::string>(isHeadUsed));
  space.Close();
}

void deliveryController::ck(QueryString &qs)
{
  response->StringResult("ck action");
}