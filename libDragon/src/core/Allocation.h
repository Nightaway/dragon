#ifndef __ALLOCATION_H__
#define __ALLOCATION_H__

#include "Macro.h"
#include <cstdlib>

NS_DRAGON

class Malloced {

public:
	static void *New(unsigned size) {
		return malloc(size);
	}

	static void Delete(void *addr) {
		return free(addr);
	}
};


NS_END


#endif
