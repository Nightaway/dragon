#ifndef __BZ2_H__
#define __BZ2_H__

#include "../core/Macro.h"

#include <string>

NS_DRAGON

	class Bzip2 {
	public:
		static std::string Compress(std::string str);
		static std::string Decompress(std::string str);
	};

NS_END

#endif