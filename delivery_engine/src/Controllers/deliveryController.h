#ifndef __DELIVERY_CONTROLLER_H__
#define __DELIVERY_CONTROLLER_H__

#include <dragon.h>

class deliveryController :
		public dragon::Controller
{
	DECLARE_CONTROLLER(deliveryController)
        DECLARE_ACTION_MAP()

public:
	void ad(dragon::QueryString &qs);
	void lg(dragon::QueryString &qs);
	void ck(dragon::QueryString &qs);
};

#endif
