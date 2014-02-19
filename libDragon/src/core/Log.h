#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <cstdio>
#include "Macro.h"

#define ITEM_BEGIN "["
#define ITEM_END   "]"
#define ITEM_SEPERAPOR " : "

NS_DRAGON

enum LogLevel {
	kLogLevelInfo = 0,
	kLogLevelDebug
};

class Log {
	const char *fileName_;
	FILE *file_;
public:
	Log() {}
	~Log() {}

	void Open(const char *fileName);
	void Close();

	void LogDebug(std::string msg);
	void LogFmt(const char *fmt, ...);
	void LogInfo(std::string msg);
};

NS_END

#endif
