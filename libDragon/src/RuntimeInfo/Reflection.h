#ifndef __REFLECTION_H__
#define __RELECTION_H__

#include "../DragonConfig.h"

NS_DRAGON
	NS_CORE

class RuntimeInfo {
public:
	static Object::cls_tbl_t *GetClsTbl(const std::string &name);
	static Object::cls_info_t *GetClsInfo(const std::string &name);
	static void PrintAllClass();
	static Object *NewInstance(const std::string &name);
	static void PrintParentClass(const std::string &name);

	template<class T>
	static void PrintParentClass()
	{
		// 打印所有ClsC类的所有父类
		Object::cls_tbl_t  *h = &T::__cls_tbl;
		for (h; h != 0; h = h->pParent)
		{
			std::cout << h->pClass->clsId << std::endl;
			std::cout << h->pClass->clsName << std::endl;
		}
	}
};

template<class T>
struct RuntimeMapping {
	std::string type_name;
	T (*convert_handler)();
};

	NS_END
NS_END


#endif