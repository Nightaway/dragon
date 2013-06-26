#include "../Macro.h"

#include <string>
#include <vector>

#include "Memory.h"
#include "RuntimeInfo.h"
#include "TypeMapping.h"
#include "Object.h"

USING_NS_DRAGON
USING_NS_CORE

Object::cls_info_t Object::__cls_info = {tagObject, "Object", Object::New}; 
Object::cls_tbl_t Object::__cls_tbl = {&__cls_tbl, 0, 0, &__cls_info};
Object::cls_tbl_t *Object::__last_cls_tbl = 0;

Object *Object::New()
{
	return new Object;
}

void Object::__init_cls_info()  
{
	__last_cls_tbl = &__cls_tbl;
} 

void Object::__init_method_info()
{
	//AddMethod(0, Object, Fooi);
}

void Object::__init_property_info()
{

}

void Object::serialze(Memory &m)
{
		// 打印所有父类
		CLSTBL_FOREACH(h)
		{
			PROPERTY_FOREACH(h)
			{
				char *addr = (char *)this;
				for (int i=0; i<type_map_size; ++i)
				{
					if (iter->type_name == type_map[i].type_name)
					{
						type_map[i].mapping_handler(m, addr+iter->offset);
					}
				}
			}
		}
}

