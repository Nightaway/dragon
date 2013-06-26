#include <iostream>

#include "ClassABC.h"

USING_NS_DRAGON
USING_NS_CORE

IMPLEMENT_CLASS(tagClsA, Object, ClsA)
IMPLEMENT_CLASS(tagClsB, Object, ClsB)
IMPLEMENT_CLASS(tagClsC, ClsA, ClsC)

void ClsA::__init_method_info()
{

}

void ClsA::__init_property_info()
{
	AddProperty(ClsA, i, int);
	AddProperty(ClsA, d, double);
}

void ClsB::__init_method_info()
{

}

void ClsB::__init_property_info()
{

}

void ClsC::__init_method_info()
{

}

void ClsC::__init_property_info()
{
	AddProperty(ClsC, k, int);
	AddProperty(ClsC, str, std::string);
	AddProperty(ClsC, ids, std::vector<int>);
}