#ifndef __DRAGON_STRING_REF_H__
#define __DRAGON_STRING_REF_H__

#include "../config.h"
#include "Macro.h"

NS_DRAGON

class StringRef {
	const char *str_;
	unsigned    len_; 

	public:
	StringRef();
	StringRef(const char *str, unsigned len) 
		: str_(str), len_(len)
	{}

	const char *string()
	{
		return str_;
	}

	unsigned length()
	{
		return len_;
	}	
};

NS_END



#endif
