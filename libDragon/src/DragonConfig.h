#ifndef __DRAGON_CONFIG_H__
#define __DRAGON_CONFIG_H__

#ifdef WIN32

namespace dragon {
	const char path_delimiter = '\\';
};

#else

namespace dragon {
	const char path_delimiter = '/';
};

#endif


#endif