#ifndef __DB_CONTEXT_H__
#define __DB_CONTEXT_H__

#include <mysql_driver.h>

NS_DRAGON

class MySQLContext {
	sql::Driver *drv_;

public:

	sql::Connection *Connect(const std::string &host, 
											  const std::string &user, 
											  const std::string &pass,
											  const std::string &scheme = "");
};

v8::Handle<v8::Object> WrapMySQLContext(MySQLContext *ctx, v8::Isolate *isolate);

NS_END

#endif