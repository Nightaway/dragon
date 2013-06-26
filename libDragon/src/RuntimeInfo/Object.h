#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "../DragonConfig.h"

NS_DRAGON
	NS_CORE

#define tagObject 0

class Memory;
class Object {
public:
	typedef runtime_info_trait<Object>::cls_info_t cls_info_t;
	typedef runtime_info_trait<Object>::cls_tbl_t   cls_tbl_t;
	static cls_info_t __cls_info;
	static cls_tbl_t __cls_tbl;
	static Object *New();

	static cls_tbl_t *__last_cls_tbl;

public:
	static void __init_cls_info();
	static void __init_method_info();
	static void __init_property_info();

	Object() {}
	virtual ~Object() {}

	bool is_a(const std::string &cls_name)
	{
		Object::cls_tbl_t  * h = &Object::__cls_tbl;
		for (h; h != 0; h = h->pParent)
		{
			if (cls_name == h->pClass->clsName)
				return true;
		}
		return false;
	}

	template <class T>
	bool setProperty(const std::string &name, const T &v)
	{
		// 打印所有父类
		Object::cls_tbl_t  *h = &__cls_tbl;
		for (h; h != 0; h = h->pParent)
		{
			property_tbl_t::iterator iter = h->pClass->property_tbl.begin();
			property_tbl_t::iterator end = h->pClass->property_tbl.end();
			for (; iter != end; ++iter)
			{
				char *pProperty = (char *)this;
				if (name == iter->name)
				{
					(*((T *)(pProperty + iter->offset))) = v;
					return true;
				}
			}
		}
		return false;
	}

	template <class T>
	bool getProperty(const std::string &name, T & v)
	{
		// 打印所有父类
		Object::cls_tbl_t  *h = &__cls_tbl;
		for (h; h != 0; h = h->pParent)
		{
			property_tbl_t::iterator iter = h->pClass->property_tbl.begin();
			property_tbl_t::iterator end = h->pClass->property_tbl.end();
			for (; iter != end; ++iter)
			{
				char *pProperty = (char *)this;
				if (name == iter->name)
				{
					 v = (*((T *)(pProperty + iter->offset)));
					return true;
				}
			}
		}
		return false;
	}

	unsigned int bytes()
	{
		unsigned int size = 0;
		// 打印所有父类
		Object::cls_tbl_t  *h = &__cls_tbl;
		for (h; h != 0; h = h->pParent)
		{
			property_tbl_t::iterator iter = h->pClass->property_tbl.begin();
			property_tbl_t::iterator end = h->pClass->property_tbl.end();
			for (; iter != end; ++iter)
			{
				size += iter->size;
			}
		}
		return size;
	}

	void serialze(Memory &m);
};

template <class T, class T2, class T3>
void invoke(const std::string &cls_name, const std::string &method_name, T2 i)
{
	Object::cls_tbl_t  * h = Object::__cls_tbl.pHead;
	for (h; h != 0; h = h->pNext)
	{
		if ( h->pClass->clsName == cls_name)
		{
			T3 *obj = (T3 *)h->pClass->New();

			method_tbl_t::iterator iter  = obj->__cls_info.method_tbl.begin();
			method_tbl_t::iterator end = obj->__cls_info.method_tbl.end();
			for (iter; iter!=end; ++iter)
			{
				if (method_name == iter->name)
				{
					 T pM = (T) iter->method;
					 (obj->*pM)(i);
				}
			}
		}
	}
}


	NS_END
NS_END
#endif