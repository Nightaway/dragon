#include "../Macro.h"

#include <string>
#include <vector>
#include <iostream>

#include "RuntimeInfo.h"
#include "Object.h"
#include "Reflection.h"

USING_NS_DRAGON
USING_NS_CORE

Object::cls_tbl_t *RuntimeInfo::GetClsTbl(const std::string &name)
{
	Object::cls_tbl_t  *h = Object::__cls_tbl.pHead;
	for (h; h != 0; h = h->pNext)
	{
		if (h->pClass->clsName == name)
		{
			return h;
		}
	}
	return NULL;
}

Object::cls_info_t *RuntimeInfo::GetClsInfo(const std::string &name)
{
	Object::cls_tbl_t  *h = GetClsTbl(name);

	if (h == NULL) {
		return NULL;
	} else {
		return h->pClass;
	}
}

void RuntimeInfo::PrintAllClass()
{
		// 打印所有类
		Object::cls_tbl_t  *h = Object::__cls_tbl.pHead;
		for (h; h != 0; h = h->pNext)
		{
			std::cout << h->pClass->clsId << std::endl;
			std::cout << h->pClass->clsName << std::endl;
		}
}

Object *RuntimeInfo::NewInstance(const std::string &name)
{
	Object::cls_info_t *info = GetClsInfo(name);
	if (info == NULL) {
		return NULL;
	} else {
		return info->New();
	}
}

void RuntimeInfo::PrintParentClass(const std::string &name)
{
	// 打印所有ClsC类的所有父类
	Object::cls_tbl_t  *h = GetClsTbl(name);
	for (h; h != 0; h = h->pParent)
	{
		std::cout << h->pClass->clsId << std::endl;
		std::cout << h->pClass->clsName << std::endl;
	}
}