#ifndef __DATE_TIME_H__
#define __DATE_TIME_H__

#include "../core/Macro.h"
#include <string>

NS_DRAGON

class DateTime
{
public:
	static std::string YearFromNow(size_t times);
};

NS_END

#endif