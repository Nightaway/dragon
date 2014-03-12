#include <gtest/gtest.h>
#include <core/Space.h>
#include <core/Table.h>
#include <Models/AdInfo.h>
#include <cstdio>

NS_USING_DRAGON

typedef Table<AdInfo> AdInfoTable;

TEST(space, test1) {
	NamedSemiSpace space("test", 1024 * 1024 * 4);
	space.Create();

	void *addr = space.Address();
	void *half = space.HalfAddress();
	printf("%X, %X\n", addr, half);

	AdInfoTable adInfoTbl(100);
	adInfoTbl.Dump(space);

	AdInfo ad1;
	ad1.id = 1;
	ad1.name = "Motu";
	ad1.width = 200;
	ad1.height = 300;
	ad1.Dump(space);
	adInfoTbl.Put(ad1.id, &ad1);

	AdInfo *ad2 = adInfoTbl.Get(1);
	printf("id:%d\n", ad2->id);
	
	space.Destroy();
}
