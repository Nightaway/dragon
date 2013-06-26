#ifndef __TRUNTIME_INFO_H__
#define __TRUNTIME_INFO_H__

#include <Dragon.h>

#define tagClsA 1
#define tagClsB 2
#define tagClsC 3

class ClsA : public dragon::core::Object {
public:
	DECLARE_CLASS(ClsA);
	
	int i;
	double d;
public:
	void foo()
	{

	}

	void Foo() 
	{
		printf("fooooo!\n");
	}

	void Fooi(int i)
	{ 
		printf("foooo! %d\n", i);
	}
};

class ClsB : public dragon::core::Object {
public:
	DECLARE_CLASS(ClsB);

public:

};

class ClsC : public ClsA {
public:
	DECLARE_CLASS(ClsB);

public:
	int k;
	std::string str;
	std::vector<int> ids;
};

#endif
