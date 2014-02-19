#include "Log.h"

#include <cstdarg>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;
NS_USING_DRAGON

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

void Log::LogDebug(std::string msg)
{
	std::stringstream ss;
	ss << "[" << getppid() << "]" << TimeString() << "[" << "debug" << "]" << ITEM_SEPERAPOR << msg << std::endl;
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
			LogDebug(std::string(p, strlen(p)));
		
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

void Log::LogInfo(std::string msg)
{

}
