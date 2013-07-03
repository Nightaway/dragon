#include "../Macro.h"

#include <v8.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "DBContext.h"

USING_NS_DRAGON;
USING_NS_V8;

sql::Connection *MySQLContext::Connect(const std::string &host,
																    const std::string &user, 
																    const std::string &pass,
																	const std::string &scheme)
{
	static sql::Connection *conn_ = NULL;
	if (conn_ == NULL || conn_->isClosed())
	{
				drv_ = sql::mysql::get_driver_instance();
				conn_ = drv_->connect(host, user, pass);

				if (!scheme.empty()) {
					conn_->setSchema(scheme);
				}
	}
	return conn_;
}

static Persistent<ObjectTemplate>MySQLContextTempl;
static Persistent<ObjectTemplate>MySQLConnectionTempl;
static Persistent<ObjectTemplate>MySQLPreparedStatement;
static Persistent<ObjectTemplate>MySQLResultSetTempl;

static Handle<Value> ConnectCallback(const Arguments& args);
static Handle<Value> SetSchemeCallback(const Arguments& args);
static Handle<Value> prepareStatementCallback(const Arguments& args);
static Handle<Value> executeQueryCallback(const Arguments& args);
static Handle<Value> nextCallback(const Arguments& args);
static Handle<Value> getCallback(const Arguments& args);
static Handle<Value> setCallback(const Arguments& args);
static Handle<Value> GetStringCallback(const Arguments& args);
//static Handle<Value> disposeCallback(const Arguments& args);

template <class T>
T *Unwrap(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<T *>(ptr);
}

template <class T>
Handle<Value> disposeCallback(const Arguments& args)
{
	if (args.Length() != 0) {
		return Undefined();
	}

	T *ptr = Unwrap<T>(args.Holder());

	delete ptr;

	return Undefined();
}

static Handle<ObjectTemplate> MakeMySQLContextTmpl(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> tmpl = ObjectTemplate::New();
	tmpl->SetInternalFieldCount(1);
	
	tmpl->Set(String::New("Connect"), FunctionTemplate::New(ConnectCallback));

	return scope.Close(tmpl);
}

static Handle<ObjectTemplate> MakeMySQLPreparedStatementTmpl(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> tmpl = ObjectTemplate::New();
	tmpl->SetInternalFieldCount(1);

	tmpl->Set(String::New("ExecuteQuery"), FunctionTemplate::New(executeQueryCallback));
	tmpl->Set(String::New("Set"), FunctionTemplate::New(setCallback));
	tmpl->Set(String::New("Dispose"), FunctionTemplate::New(disposeCallback<sql::PreparedStatement>));

	return scope.Close(tmpl);
}

static Handle<ObjectTemplate> MakeMySQLConnectionTmpl(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> tmpl = ObjectTemplate::New();
	tmpl->SetInternalFieldCount(1);
	
	tmpl->Set(String::New("SetScheme"), FunctionTemplate::New(SetSchemeCallback));
	tmpl->Set(String::New("PrepStmt"), FunctionTemplate::New(prepareStatementCallback));

	return scope.Close(tmpl);
}

static Handle<ObjectTemplate> MakeMySQLResultSet(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> tmpl = ObjectTemplate::New();
	tmpl->SetInternalFieldCount(1);

	tmpl->Set(String::New("Next"), FunctionTemplate::New(nextCallback));
	tmpl->Set(String::New("Get"), FunctionTemplate::New(getCallback));
	tmpl->Set(String::New("GetString"), FunctionTemplate::New(GetStringCallback));
	tmpl->Set(String::New("Dispose"), FunctionTemplate::New(disposeCallback<sql::ResultSet>));

	return scope.Close(tmpl);
}

Handle<Object> dragon::WrapMySQLContext(MySQLContext *ctx, Isolate *isolate)
{
	HandleScope scope(isolate);
	if (MySQLContextTempl.IsEmpty()) {
		Handle<ObjectTemplate> temp = MakeMySQLContextTmpl(isolate);
		MySQLContextTempl.Reset(isolate, temp);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate, MySQLContextTempl);
	Handle<Object> obj = objT->NewInstance();
	Handle<External> ptr = External::New(ctx);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

MySQLContext *UnwrapMySQLContext(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<MySQLContext *>(ptr);
}

Handle<Object> WrapMySQLConnection(Isolate *isolate, sql::Connection *conn)
{
	HandleScope scope(isolate);
	if (MySQLConnectionTempl.IsEmpty()) {
		Handle<ObjectTemplate> tmpl = MakeMySQLConnectionTmpl(isolate);
		MySQLConnectionTempl.Reset(isolate, tmpl);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate, MySQLConnectionTempl);
	Handle<Object> obj = objT->NewInstance();
	Handle<External> ptr = External::New(conn);
	obj->SetInternalField(0, ptr);
	return scope.Close(obj);
}

sql::Connection  *UnwrapMySQLConnection(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<sql::Connection *>(ptr);
}

Handle<Object> WrapMySQLPreparedStmt(sql::PreparedStatement *prep_stmt, Isolate *isolate)
{
	HandleScope scope(isolate);
	if (MySQLPreparedStatement.IsEmpty()) {
		Handle<ObjectTemplate> temp = MakeMySQLPreparedStatementTmpl(isolate);
		MySQLPreparedStatement.Reset(isolate, temp);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate, MySQLPreparedStatement);
	Handle<Object> obj = objT->NewInstance();
	Handle<External> ptr = External::New(prep_stmt);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

sql::PreparedStatement  *UnwrapMySQLPreparedStmt(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<sql::PreparedStatement *>(ptr);
}

Handle<Object> WrapMySQLResultSet(sql::ResultSet *rs, Isolate *isolate)
{
	HandleScope scope(isolate);
	if (MySQLResultSetTempl.IsEmpty()) {
		Handle<ObjectTemplate> temp = MakeMySQLResultSet(isolate);
		MySQLResultSetTempl.Reset(isolate, temp);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate, MySQLResultSetTempl);
	Handle<Object> obj = objT->NewInstance();
	Handle<External> ptr = External::New(rs);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

sql::ResultSet  *UnwrapMySQLResultSet(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<sql::ResultSet *>(ptr);
}

static Handle<Value> ConnectCallback(const Arguments& args) 
{
	MySQLContext *ctx = UnwrapMySQLContext(args.Holder());

	if (args.Length() != 3 && args.Length() != 4) {
		return Undefined();
	}

	Handle<Value> host = args[0];
	if (!host->IsString()) {
		return Undefined();
	}
	Handle<Value> user = args[1];
	Handle<Value> pass = args[2];

	if (args.Length() == 4) {
		Handle<Value> scheme = args[3];
		String::Utf8Value strHost(host), strUser(user), strPass(pass), strScheme(scheme);
		sql::Connection *conn = ctx->Connect(*strHost, *strUser, *strPass, *strScheme);
		Handle<Object> conn_obj = WrapMySQLConnection(args.GetIsolate(), conn);
		return conn_obj;
	} else {
		String::Utf8Value strHost(host), strUser(user), strPass(pass);
		sql::Connection *conn = ctx->Connect(*strHost, *strUser, *strPass);
		Handle<Object> conn_obj = WrapMySQLConnection(args.GetIsolate(), conn);
		return conn_obj;
	}
}

static Handle<Value> SetSchemeCallback(const Arguments& args)
{
	sql::Connection *conn = UnwrapMySQLConnection(args.Holder());

	if (args.Length() != 1) {
		return Undefined();
	}

	Handle<Value> scheme = args[0];
	if (!scheme->IsString()) {
		return Undefined();
	}
	String::Utf8Value strScheme(scheme);
	conn->setSchema(*strScheme);
	return Undefined();
}

static Handle<Value> prepareStatementCallback(const Arguments& args)
{
	sql::Connection *conn = UnwrapMySQLConnection(args.Holder());

	if (args.Length() != 1) {
		return Undefined();
	}

	Handle<Value> query = args[0];
	if (!query->IsString()) {
		return Undefined();
	}

	String::Utf8Value strQuery(query);
	sql::PreparedStatement *prep_stmt = NULL;
	try {
		prep_stmt = conn->prepareStatement(*strQuery);
	} catch (sql::SQLException &e) {
		std::cout << e.what();
	}
	Handle<Object> prep_stmt_obj = WrapMySQLPreparedStmt(prep_stmt, args.GetIsolate());
	return prep_stmt_obj;
}

static Handle<Value> executeQueryCallback(const Arguments& args)
{
	sql::PreparedStatement *prep_stmt = UnwrapMySQLPreparedStmt(args.Holder());

	if (args.Length() != 0) {
		return Undefined();
	}
	sql::ResultSet *rs  = prep_stmt->executeQuery();
	Handle<Object> rs_obj = WrapMySQLResultSet(rs, args.GetIsolate());

	return rs_obj;
}

static Handle<Value> nextCallback(const Arguments& args)
{
	sql::ResultSet *rs = UnwrapMySQLResultSet(args.Holder());

	if (args.Length() != 0) {
		return Undefined();
	}

	bool b = rs->next();
	Handle<Boolean> boolean = Boolean::New(b);
	return boolean;
}

static Handle<Value> GetStringCallback(const Arguments& args)
{
	sql::ResultSet *rs = UnwrapMySQLResultSet(args.Holder());

	if (args.Length() != 1) {
		return Undefined();
	}

	Handle<Value> idx = args[0];
	if (!idx->IsUint32()) {
		return Undefined();
	}
	const unsigned uint = idx->ToUint32()->Uint32Value();
	return String::New(rs->getString(uint).c_str());
}

static Handle<Value> getCallback(const Arguments& args)
{
	sql::ResultSet *rs = UnwrapMySQLResultSet(args.Holder());

	if (args.Length() != 1) {
		return Undefined();
	}

	Handle<Value> name = args[0];
	if (!name->IsString()) {
		return Undefined();
	}

	const String::Utf8Value strName(name);

	sql::ResultSetMetaData *rsm =rs->getMetaData();
	for (unsigned i=1; i<rsm->getColumnCount(); ++i)
	{
	//	std::cout << rsm->getColumnName(i) << std::endl;
		if (*strName != rsm->getColumnName(i)) {
			continue;
		}
		switch ( rsm->getColumnType(i))
		{
		case sql::DataType::BIGINT:
		case sql::DataType::INTEGER:
		case sql::DataType::SMALLINT:
		case sql::DataType::TINYINT:
		case sql::DataType::MEDIUMINT:
			return Integer::New(rs->getInt(*strName));
		case sql::DataType::DATE:
			std::cout << "Date" << std::endl;
		break;
		case sql::DataType::CHAR:
		case sql::DataType::VARCHAR :
			return  String::New(rs->getString(*strName).c_str());
		case sql::DataType::DOUBLE :
	//		std::cout << "double" << rs->getDouble(*strName) << std::endl;
			return Number::New(rs->getDouble(*strName));

		//case sql::DataType::UNKNOWN:
		//	return  String::New(rs->getString(*strName).c_str());
		}
	}
	return Undefined();
}

static Handle<Value> setCallback(const Arguments& args)
{
	sql::PreparedStatement *prep_stmt = UnwrapMySQLPreparedStmt(args.Holder());

	if (args.Length() != 2) {
		return Undefined();
	}

	Handle<Value> idx = args[0];
	Handle<Value> v    = args[1];

	try {
		int32_t  i_idx= idx->Int32Value();
		if (v->IsString()) {
			String::Utf8Value str_v(v);
			prep_stmt->setString(i_idx, *str_v);
		} else  if (v->IsInt32()) {
			int32_t i_v = v->Int32Value();
			prep_stmt->setInt(i_idx, i_v);
		}
	} catch (sql::SQLException &e) {
		std::cout << e.what();
	}
	return Undefined();
}

