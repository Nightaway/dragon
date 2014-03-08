#include "Log.h"

#include "../../wrapper/Wrapper.h"

#include <cstdarg>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

NS_USING_DRAGON
NS_USING_V8

Log::Log(JavaScriptCompiler &jsCompiler, 
	 LogLevel logLevel, 
	 const char *name) : JavaScriptModule(jsCompiler),
			     logLevel_(logLevel)
			  
{
	_name_ = name;
	_v_major_ = 0;
	_v_minor_ = 1;
}

Log::~Log()
{

}

void Log::Init()
{
	jsCompiler_.AddModule(_name_, this);
}

void Log::Dispose()
{

}

static Persistent<ObjectTemplate> logTemplate;
static Handle<ObjectTemplate> MakeLogTemplate(Isolate *isolate);

static void LogCallback(const v8::FunctionCallbackInfo<Value>& args) 
{
	if (args.Length() !=1) return ;

	Log* log = Unwrap<Log>(args.Holder());	

	Handle<Value> msg_value = args[0];
	v8::String::Utf8Value msg_str(msg_value);

	log->LogMsg(std::string(*msg_str, strlen(*msg_str)));
}

Handle<Object> Log::Wrap()
{
	Isolate *isolate = jsCompiler_.GetIsolate();
	HandleScope scope(isolate);

	if (logTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeLogTemplate(isolate);
		logTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, logTemplate);
	Handle<Object> obj = local->NewInstance();
	Handle<External> log_ptr = External::New(this);
	obj->SetInternalField(0, log_ptr);
	return scope.Close(obj);
}

Handle<ObjectTemplate> MakeLogTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);
	objTemplate->Set(v8::String::New("log"), FunctionTemplate::New(LogCallback));
	return scope.Close(objTemplate);
}

static char *fmtMessage(const char *fmt, ...)
{
	int n;
	int size = 200; 
	char *p, *np;
	va_list ap;
	
	if ((p = (char *)malloc(size)) == NULL)
		return NULL;

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n > -1 && n < size)
			return p;
		
		if (n > -1)	
			size = n + 1;
		else 
			size *= 2;
		
		if ((np == realloc(p, size)) == NULL) {
			free(p);
			return NULL;
		} else {
			p = np;
		}
	}
}

std::string TimeString()
{
	std::string timeString;
	ptime now = second_clock::local_time();
	timeString += ITEM_BEGIN;
	timeString += to_simple_string(now);
	timeString += ITEM_END;
	return timeString;
}

void Log::Open(const char *fileName)
{
	file_ = fopen(fileName, "a");
	
	if (file_ == NULL) {
		std::string msg = "Error:failed open ";
		msg += fileName;
		throw std::runtime_error(msg);
	}
}

void Log::Close()
{
	fclose(file_);
}

void Log::LogMsg(std::string msg)
{

	std::string logLevel = "Info";

	switch (logLevel_) {
		case kLogLevelInfo:
		logLevel = "Info";
		break;
		
		case kLogLevelError:
		logLevel = "Error";
		break;

		case kLogLevelDebug:
		logLevel = "Debug";
		break;

		default:
		UNREACHABLE();
		break;
	}

	std::stringstream ss;
	ss << "[" << getppid() << "]" << TimeString() << "[" << logLevel << "]" << ITEM_SEPERAPOR << msg << std::endl;
	std::string str = ss.str();
	fwrite(str.c_str(), str.length(), 1, file_);
}

void Log::LogFmt(const char *fmt, ...)
{
	int n;
	int size = 200; 
	char *p, *np;
	va_list ap;
	
	if ((p = (char *)malloc(size)) == NULL)
		return ;

	while (1) {
		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n > -1 && n < size)
			LogMsg(std::string(p, strlen(p)));
		
		if (n > -1)	
			size = n + 1;
		else 
			size *= 2;
		
		if ((np == realloc(p, size)) == NULL) {
			free(p);
			return ;
		} else {
			p = np;
		}
	}
}
