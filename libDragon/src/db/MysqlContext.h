#ifndef __MYSQL_CONTEXT_H__
#define __MYSQL_CONTEXT_H__

#include "../core/Macro.h"
#include "../core/Context.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

NS_DRAGON

class MysqlContext : public Context {
public:
	MysqlContext(){}
	~MysqlContext() {}

	sql::Connection *Connect(const char *host,
				 			 const char *user,
							 const char *pass,
							 const char *db)
	{	
		static sql::Connection *conn = NULL;
		if (conn == NULL || conn->isClosed()) {
			driver_ = sql::mysql::get_driver_instance();
			conn = driver_->connect(host, user, pass);
			conn->setSchema(db);
			conn->setClientOption("characterSetResults", "utf8");
		}
		return conn;
	}

private:
	sql::Driver *driver_;
};

NS_END

#endif
