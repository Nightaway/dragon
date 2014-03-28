#include <gtest/gtest.h>
#include <core/Space.h>
#include <core/Table.h>
#include <core/String.h>
#include <core/Heap.h>
#include <Models/AdInfo.h>
#include <cstdio>

NS_USING_DRAGON

typedef Table<AdInfo> AdInfoTable;

TEST(space, integer) {
  NamedSemiSpace space("test", 1024);
  space.Create();
  
  Ref<int> i = Ref<int>::New(10, space);
  ASSERT_EQ(*i, 10);

  Ref<double> d = Ref<double>::New(20.20, space);
  ASSERT_EQ(*d, 20.20);

  Ref<Integer> integer = Integer::New(20, space);
  ASSERT_EQ(integer->Value(), 20);

  space.Destroy();
}

TEST(space, object_test1) {
	printf("Object:%d, String:%d\n", sizeof(Object), sizeof(String));
}

TEST(space, obj_dump_test1) {
	NamedSemiSpace space("test1", 1024 * 1024 * 4);
	space.Create();

	void *addr = space.Address();
	void *half = space.HalfAddress();
	void *head = space.HeadAddress();
	printf("addr:%X, half:%X, head:%X\n", addr, half, head);

	AdInfo ad1;
	ad1.banner_id = 1;
	//ad1.name = std::string("Motu");
	ad1.width = 200;
	//ad1.height = 300;
	ad1.Dump(space);
	
	space.SetPos(0);
	AdInfo ad2;
	ad2.Stuff(space);
	ASSERT_EQ(1, ad2.banner_id);
	//ASSERT_STREQ("Motu", ad2.name.c_str());
	ASSERT_EQ(200, ad2.width);
	//ASSERT_EQ(300, ad2.height);

	space.Switch();

	addr = space.Address();
	half = space.HalfAddress();
	head = space.HeadAddress();
	printf("addr:%X, half:%X, head:%X\n", addr, half, head);

	AdInfo ad3;
	ad3.banner_id = 3;
	//ad3.name = std::string("ajs");
	ad3.width = 400;
	//ad3.height = 400;
	ad3.Dump(space);

	AdInfo ad4;
	space.SetPos(0);
	ad4.Stuff(space);
	ASSERT_EQ(3, ad4.banner_id);
	//ASSERT_STREQ("ajs", ad4.name.c_str());
	ASSERT_EQ(400, ad4.width);
	//ASSERT_EQ(400, ad4.height);
	
	space.Destroy();
}

TEST(space, table_test1) {
	NamedSemiSpace space("test3", 1024 * 4);
	space.Create();

	OffsetTable table(100, space);
	printf("offset:%d, cap:%d\n",space.GetPos(), table.GetCapacity());
	AdInfo ad1;
	ad1.banner_id = 1;
	//ad1.name = std::string("rpl");
	ad1.Dump(space);
	table.Put(ad1.banner_id, space.GetPos());
	printf("offset:%d\n",space.GetPos());
	AdInfo ad2;
	ad2.banner_id = 1;
	//ad2.name = std::string("mmts");
	ad2.Dump(space);
	table.Put(ad2.banner_id, space.GetPos());
	printf("offset:%d\n",space.GetPos());

	space.Close();
}

TEST(space, obj_ref_test1) {
	/*StringHeap heap("StringHeap", 4096);
	heap.Create();
	heap.Clear();

	NamedSemiSpace space("test2", 1024 * 1024 * 4);
	space.Create();

	Ref<AdInfo> ad1 = Ref<AdInfo>::New(space);
	ad1->id = 1;		
	ad1->name = StringRef("22", strlen("22"));
	printf("Heap pos:%d\n", heap.GetPos());
	ad1->text = heap.Allocate("11", strlen("11"));
	//heap.AddRoot(&(ad1->text));

	printf("Heap pos:%d\n", heap.GetPos());
	heap.Allocate("121", strlen("121"));
	printf("Heap pos:%d\n", heap.GetPos());
	printf("id:%d, name:%s, text:%s\n", ad1->id, ad1->name.string(), ad1->text->GetString());
	heap.Close();
	space.Destroy();*/
}

TEST(space, open_test1) {
	/*StringHeap heap("StringHeap", 4096);
	heap.Open();
	heap.PrintObjs();
	heap.Close();*/
}


