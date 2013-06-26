#include <iostream>

#include "Class/ClassABC.h"

#include <gtest/gtest.h>

#define INIT_ABC() \
	USING_NS_DRAGON;\
	USING_NS_CORE;\
	InitClass(Object);\
	InitClass(ClsA);\
	InitClass(ClsB);\
	InitClass(ClsC);

TEST(ClassABC, PrintAllClass) {
	INIT_ABC()

	const char *ClsNameList[] = {
		"Object", "ClsA", "ClsB", "ClsC"
	};

	const int ClsTags[] = {tagObject, tagClsA, tagClsB, tagClsC};

	/*
				循环链表打印所有类型信息
	*/
	Object::cls_tbl_t *tbl = Object::__cls_tbl.pHead;
	for (int i=0; tbl != NULL; tbl = tbl->pNext, ++i)
	{
		EXPECT_EQ(tbl->pClass->clsId, ClsTags[i]);
		EXPECT_STREQ(tbl->pClass->clsName, ClsNameList[i]);
	}
}

TEST(ClassABC, is_a) {
	USING_NS_DRAGON;
	USING_NS_CORE;

	ClsA a; ClsB b; ClsC c;

	EXPECT_EQ(true, a.is_a("Object"));
	EXPECT_EQ(true, a.is_a("ClsA"));
	EXPECT_EQ(false, a.is_a("ClsB"));
	EXPECT_EQ(false, a.is_a("ClsC"));

	EXPECT_EQ(true, b.is_a("Object"));
	EXPECT_EQ(false, b.is_a("ClsA"));
	EXPECT_EQ(true, b.is_a("ClsB"));
	EXPECT_EQ(false, b.is_a("ClsC"));
}

TEST(ClassABC, NewInstance) {
	USING_NS_DRAGON;
	USING_NS_CORE;

	Object *pClsC = RuntimeInfo::NewInstance("ClsC");
	ClsC *objC = dynamic_cast<ClsC *>(pClsC);
	EXPECT_NE((ClsC *)NULL, objC);

	ClsA *objA = dynamic_cast<ClsA *>(ClsA::New());
	EXPECT_NE((ClsA *)NULL, objA);
}

TEST(ClassABC, PropertyAndSerilization) {
	USING_NS_DRAGON;
	USING_NS_CORE;

	ClsC objC;
	objC.i = 0;
	objC.str = "Hello++++++++++++++";
	objC.ids.push_back(1);
	objC.ids.push_back(5);

	/*
			k 是 ClsC 的属性
	*/
	int k = 0;
	objC.k = 10;
	EXPECT_EQ(true, objC.getProperty("k", k));
	EXPECT_EQ(10, k);

	EXPECT_EQ(true, objC.setProperty("k", 100));
	EXPECT_EQ(true, objC.getProperty("k", k));
	EXPECT_EQ(100, k);
	
	/*
			d 是 ClsC的父类ClsA中的double类型属性
	*/
	EXPECT_EQ(true, objC.setProperty("d", 10.1));
	double d;
	EXPECT_EQ(true, objC.getProperty("d", d));
	EXPECT_EQ(10.1, d);

	/*
			将objC 以二进制方式序列化到文件中，然后反序列化回来。
	*/
	size_t bytes = objC.bytes();
	size_t size    = sizeof(objC.i) + sizeof(objC.k) + sizeof(objC.d) + 
							objC.str.length() + sizeof(size_t) + objC.ids.size() * sizeof(int) + sizeof(size_t);
	EXPECT_EQ(size, bytes);

	Memory out(SERI_OUT, bytes);
	objC.serialze(out);
	out.write("objC.data");

	ClsC newC;
	Memory in(SERI_IN);
	in.read("objC.data");
	newC.serialze(in);

	EXPECT_EQ(newC.d, 10.1);
	EXPECT_EQ(newC.k, 100);
	EXPECT_EQ(newC.i,  0);
	EXPECT_EQ(newC.str, "Hello++++++++++++++");
	EXPECT_EQ(newC.ids[0], 1);
	EXPECT_EQ(newC.ids[1], 5);
}
