#ifndef __META_DATA_H__
#define __META_DATA_H__

#include <iostream>
#include <string>
#include <vector>

#define DECLARE_META_DATA(cls_name)\
	static MetaDataList __metaData;\
	static void __add_meta_data();\
	size_t __size;

#define IMPLEMENT_META_DATA(cls_name)\
	MetaDataList cls_name::__metaData;

#define INLINE_IMPL_BYTE(cls_name)\
	size_t byte() {\
		return byte_impl<cls_name>();}

#define INLINE_IMPL_SERI(cls_name)\
	void Serialize(Memory &m){\
		Serialize_impl<cls_name>(m);}

#define _INLINE_IMPL_SERI(super_cls, cls_name)\
	size_t byte() {\
		return super_cls::byte() + byte_impl<cls_name>();}\
	void Serialize(Memory &m){\
		super_cls::Serialize(m);\
		Serialize_impl<cls_name>(m);}

#define ADD_META_DATA(cls_name, mem_type, mem_name)\
		{MetaData m;\
			m.type = #mem_type;\
			m.size  = sizeof(mem_type);\
			m.offset = (int)&(( (cls_name*)(0) )->mem_name);\
			__metaData.push_back(m);}

#define DATA_MEM(mem, type)\
		(*((type *)mem))

#define BEGIN_ADD_META_DATA(cls_name)\
		void cls_name::__add_meta_data() {

#define _BEGIN_ADD_META_DATA(super_cls, cls_name)\
		void cls_name::__add_meta_data() {\
			super_cls::__add_meta_data();

#define END_ADD_META_DATA }

template  <class T>
inline T getDataMem(char *addr)
{
	return *((T *)addr);
}

class MetaData {
public:
	std::string type;
	int size;
	int offset;
};


typedef std::vector<MetaData> MetaDataList;

class Model {
public:
	DECLARE_META_DATA(Model)

protected:
	template <class T>
	size_t  byte_impl()
	{
		T *p = (T *)(this);
		p->__size = 0;
		for (MetaDataList::iterator iter = p->__metaData.begin(); iter != p->__metaData.end(); iter++)
		{
			char *pA = (char *)(p);
			pA +=iter->offset;

			if (iter->type == "std::string")
			{
				p->__size += sizeof(size_t);
				p->__size += DATA_MEM(pA, std::string).length() * sizeof(char);
			} else if (iter->type == "std::vector<int>")
			{
				p->__size += DATA_MEM(pA, std::vector<int>).size() * sizeof(std::vector<int>::value_type);
			}
			else
				p->__size += iter->size;
		}
		return p->__size;
	}

	template<class T>
	void Serialize_impl(Memory &m)
	{
		T *p = (T *)(this);
		if (m.get_io_bit() == SERI_OUT)
		{
				for (MetaDataList::iterator iter = p->__metaData.begin(); iter != p->__metaData.end(); iter++)
				{
						char *pA = (char *)p;
						pA +=iter->offset;

						if (iter->type == "int")
							m << getDataMem<int>(pA);
						else if (iter->type == "float")
							m << DATA_MEM(pA, float); 
						else if (iter->type == "double")
							m << DATA_MEM(pA, double); 
						else if (iter->type == "std::string")
							m << DATA_MEM(pA, std::string);
						//else if (iter->type == "std::vector<int>")
						//{
						//	std::cout << (DATA_MEM(pA, std::vector<int>)).size() << std::endl;
						//}
				}
			}
	}

public:
		INLINE_IMPL_BYTE(Model)
		INLINE_IMPL_SERI(Model)
};

class A : public Model {
public:
	DECLARE_META_DATA(A)

	_INLINE_IMPL_SERI(Model, A)

public:
	int a;
	double b;
	std::string c;
	float d;
	std::vector<int> vec;
};

class B : public  A {
public:
	DECLARE_META_DATA(B)

	_INLINE_IMPL_SERI(A, B)

public:
	int e;
};

IMPLEMENT_META_DATA(Model)
IMPLEMENT_META_DATA(A)
IMPLEMENT_META_DATA(B)

BEGIN_ADD_META_DATA(A)
	ADD_META_DATA(A, int, a);
	ADD_META_DATA(A, double, b);
	ADD_META_DATA(A, std::string, c);
	ADD_META_DATA(A, float, d);
	ADD_META_DATA(A, std::vector<int>, vec);
END_ADD_META_DATA

BEGIN_ADD_META_DATA(B)
	ADD_META_DATA(B, int, e);
END_ADD_META_DATA


#endif