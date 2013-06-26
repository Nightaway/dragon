#ifndef __TYPE_MAPPING_H__
#define __TYPE_MAPPING_H__

template <class T>
T &refof(char *addr)
{
	return *((T *)addr);
} 

typedef void (* type_mapping_handler)(dragon::core::Memory &m, char *addr);
typedef int (* type_byte_handler)(char *addr);

typedef struct  {
	const char  *type_name;
	type_mapping_handler mapping_handler;
	type_byte_handler byte_handler;
} type_mapping_t;

extern type_mapping_t type_map[];
extern int type_map_size;

#endif