#include <string>
#include <vector>

#include "../Macro.h"
#include "Memory.h"
#include "TypeMapping.h"

USING_NS_DRAGON;
USING_NS_CORE;

template <class T>
static void typedHandler(Memory &mem, char *addr)
{
	if (mem.get_io_bit() == SERI_OUT) {
		mem << refof<T>(addr);
	} else {
		mem >> refof<T>(addr);
	}
}

template <class T>
static int byteHandler(char *addr)
{
	return sizeof(T);
}

template <>
int byteHandler<std::string>(char *addr)
{
	std::string &str = refof<std::string>(addr);
	return str.length() + sizeof(size_t);
}

template <>
 int byteHandler<std::vector<int> >(char *addr)
{
	std::vector<int> &vec = refof<std::vector<int> >(addr);
	return vec.size() * sizeof(int) + sizeof(size_t);
}

type_mapping_t type_map[] = {
	{
		"bool", typedHandler<bool>, byteHandler<bool>
	},
	{
		"char", typedHandler<char>, byteHandler<char>
	},
	{
		"unsigned", typedHandler<unsigned>, byteHandler<unsigned>
	},
	{
		"int", typedHandler<int>, byteHandler<int>
	},
	{
		"float", typedHandler<float>, byteHandler<float>
	},
	{
		"double", typedHandler<double>, byteHandler<double>
	},
	{
		"std::string", typedHandler<std::string>, byteHandler<std::string>
	},
	{
		"std::vector<int>", typedHandler<std::vector<int> >, byteHandler<std::vector<int> >
	}
};

int type_map_size = sizeof(type_map) / sizeof(type_map[0]);