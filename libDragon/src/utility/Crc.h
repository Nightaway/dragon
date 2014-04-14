#ifndef __CRC_H__
#define __CRC_H__

#include "../core/Macro.h"
#include <string>

NS_DRAGON

	class Crc {
	public:
		static unsigned crc32(char *p, size_t nr);
	};

NS_END

#endif