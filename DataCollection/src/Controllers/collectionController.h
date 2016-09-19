#ifndef __COLLECTION_CONTROLLER_H__
#define __COLLECTION_CONTROLLER_H__

#include <dragon.h>

class collectionController :
		public dragon::Controller
{
	DECLARE_CONTROLLER(collectionController)
	DECLARE_ACTION_MAP()

public:
	void upload(dragon::QueryString &qs);
	void test(dragon::QueryString &qs);
};

#endif
