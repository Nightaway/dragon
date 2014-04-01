#include "DateTime.h"

NS_USING_DRAGON

std::string dragon::DateTime::YearFromNow(size_t times)
{
	char buf[30];
	time_t now = time(NULL) + 31536000 * times;
	tm *newtime = gmtime(&now);
	strftime(buf, 30, "%a, %m-%d-%Y %X GMT", newtime);
	return std::string(buf);
}