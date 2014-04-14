#ifndef __URL_H__
#define __URL_H__

#include <string>
#include "../core/Macro.h"

NS_DRAGON

class URL {
	public:
		static std::string encode(std::string url);
		static std::string decode(std::string url);
};

NS_END


#endif