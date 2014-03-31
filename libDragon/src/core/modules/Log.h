#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <cstdio>
#include "../Macro.h"
#include "../Module.h"

#define ITEM_BEGIN ""
#define ITEM_END   ""
#define ITEM_SEPERATOR "|~|"

NS_DRAGON

enum LogLevel {
	kLogLevelInfo = 0,
	kLogLevelError,
	kLogLevelDebug
};

class Log : public JavaScriptModule {
	const char *fileName_;
	FILE *file_;
	LogLevel logLevel_;

public:
	Log(JavaScriptCompiler &jsCompiler, LogLevel logLevel, const char *name);
	~Log();

	virtual void Init();
	virtual void Dispose();
	virtual v8::Handle<v8::Object> Wrap();

	void Open(const char *fileName);
	void Close();

	void LogMsg(std::string msg);
	void LogFmt(const char *fmt, ...);
};

NS_END

#endif
