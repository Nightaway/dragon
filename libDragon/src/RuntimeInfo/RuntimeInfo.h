#ifndef __RUNTIME_INFO_H__
#define __RUNTIME_INFO_H__

#include "../DragonConfig.h"

#define CLSTBL_FOREACH(e)\
		dragon::core::Object::cls_tbl_t  * e = &__cls_tbl;\
		for (e; e != 0; e = e->pParent)

#define PROPERTY_FOREACH(e)\
			dragon::core::property_tbl_t::iterator iter = e->pClass->property_tbl.begin();\
			dragon::core::property_tbl_t::iterator end = e->pClass->property_tbl.end();\
			for (; iter != end; ++iter)

#define DECLARE_ROOT_CLASS(cls_name)\
		static cls_tbl_t *__last_cls_tbl;

#define IMPLEMENT_ROOT_CLASS(cls_name)\
		cls_name::cls_tbl_t * cls_name::__last_cls_tbl = &cls_name::__cls_tbl_##cls_name;

#define DECLARE_CLASS(cls_name)\
	typedef dragon::core::runtime_info_trait<dragon::core::Object>::cls_info_t cls_info_t;\
	typedef dragon::core::runtime_info_trait<dragon::core::Object>::cls_tbl_t   cls_tbl_t;\
	static cls_info_t __cls_info;\
	static cls_tbl_t __cls_tbl;\
	static dragon::core::Object *New();\
	static void __init_cls_info();\
	static void __init_method_info();\
	static void __init_property_info();\
	bool is_a(const std::string cls_name);\
	template <class T>\
	bool setProperty(const std::string &name, const T &v)\
	{\
		CLSTBL_FOREACH(h)\
		{\
			PROPERTY_FOREACH(h)\
			{\
				char *pProperty = (char *)this;\
				if (name == iter->name)\
				{\
					(*((T *)(pProperty + iter->offset))) = v;\
					return true;\
				}\
			}\
		}\
		return false;\
	}\
	template <class T>\
	bool getProperty(const std::string &name, T & v)\
	{\
		CLSTBL_FOREACH(h)\
		{\
			PROPERTY_FOREACH(h)\
			{\
				char *pProperty = (char *)this;\
				if (name == iter->name)\
				{\
					 v = (*((T *)(pProperty + iter->offset)));\
					return true;\
				}\
			}\
		}\
		return false;\
	}\
	unsigned int bytes()\
	{\
		unsigned int size = 0;\
		CLSTBL_FOREACH(h)\
		{\
			PROPERTY_FOREACH(h)\
			{\
					char *addr = (char *)this;\
					for (int i=0; i<type_map_size; ++i)\
					{\
						if (iter->type_name == type_map[i].type_name)\
						{\
							size += type_map[i].byte_handler(addr+iter->offset);\
						}\
					}\
			}\
		}\
		return size;\
	}\
	void serialze(dragon::core::Memory &m);

#define IMPLEMENT_CLASS(id, parent, cls_name)\
	cls_name::cls_info_t cls_name::__cls_info = {id, #cls_name, cls_name::New};\
	cls_name::cls_tbl_t cls_name::__cls_tbl = {&Object::__cls_tbl, 0, 0, &cls_name::__cls_info};\
	Object *cls_name::New() {\
			return new cls_name; }\
	void cls_name::__init_cls_info()  {\
		__cls_tbl.pParent = &parent::__cls_tbl;\
		__last_cls_tbl->pNext = &__cls_tbl; \
		__last_cls_tbl = &__cls_tbl;} \
	bool cls_name::is_a(const std::string cls_name)\
	{\
		CLSTBL_FOREACH(h)\
		{\
			if (cls_name == h->pClass->clsName)\
				return true;\
		}\
		return false;\
	}\
	void cls_name::serialze(Memory &m)\
	{\
		CLSTBL_FOREACH(h)\
		{\
			PROPERTY_FOREACH(h)\
			{\
					char *addr = (char *)this;\
					for (int i=0; i<type_map_size; ++i)\
					{\
						if (iter->type_name == type_map[i].type_name)\
						{\
							type_map[i].mapping_handler(m, addr+iter->offset);\
						}\
					}\
			}\
		}}

#define AddProperty(cls_name, property_name, type)\
		{	property_info_t p;\
			p.name = #property_name;\
			p.type_name = #type;\
			p.size  = sizeof(type);\
			p.offset = (long long)&(( (cls_name*)(0) )->property_name);\
			__cls_info.property_tbl.push_back(p);}

#define AddPropertyEx(cls_name, property_name, _size, type)\
		{	property_info_t p;\
			p.name = #property_name;\
			p.type_name = #type;\
			p.size  = _size;\
			p.offset = (long long)&(( (cls_name*)(0) )->property_name);\
			__cls_info.property_tbl.push_back(p);}

NS_DRAGON
NS_CORE

	typedef enum {
		eTagVoid,
		eTagInt,
		eTagUInt,
		eTagFloat,
		eTagDouble
	} runtime_type_t;

	class Object;
	typedef void (Object::*method_t)();
	typedef struct  {
		int tag;
		const char *name;
		method_t method;
	} method_info_t;

	#define AddMethod(idx, cls_name, method_name) \
		{\
			method_info_t m;\
			m.tag = idx;\
			m.name = #method_name;\
			m.method = (method_t)&cls_name::method_name;\
			__cls_info.method_tbl.push_back(m);\
		}

	typedef std::vector<method_info_t> method_tbl_t;

	class property_info_t {
	public:
		std::string name;
		std::string type_name;
		unsigned size;
		int offset;
	};

	typedef std::vector<property_info_t> property_tbl_t;

	template <class T>
	struct cls_info_template {
				int clsId;
				const char *clsName;

				T *(*New)();
				method_tbl_t method_tbl;
				property_tbl_t property_tbl;
	};

	template  <class T>
	inline T getProperty(char *addr)
	{
		return *((T *)addr);
	}

	#define InitClass(cls_name) \
		cls_name::__init_cls_info();\
		cls_name::__init_method_info();\
		cls_name::__init_property_info();

	template <class T>
	struct cls_tbl_template {
			struct cls_tbl_template<T> *pHead;
			struct cls_tbl_template<T> *pNext;
			struct cls_tbl_template<T> *pParent;

			T *pClass;
	};

	template <class T>
	struct runtime_info_trait {
		typedef cls_info_template<T> cls_info_t;
		typedef cls_tbl_template<cls_info_t> cls_tbl_t;
	};

	NS_END
NS_END
#endif