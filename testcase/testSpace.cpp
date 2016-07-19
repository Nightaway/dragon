#include <gtest/gtest.h>
#include <core/Space.h>
#include <core/Table.h>
#include <core/String.h>
#include <core/Heap.h>
#include <cstdio>

NS_USING_DRAGON

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

	space.Destroy();
}

TEST(space, table_test1) {
	NamedSemiSpace space("test3", 1024 * 4);
	space.Create();

	OffsetTable table(100, space);
	printf("offset:%d, cap:%d\n",space.GetPos(), table.GetCapacity());

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


