#include "../../Macro.h"
#include "../../Error.h"

#include <string>
#include <ctime>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <v8.h>

#include "../../Web/HttpRequest.h"
#include "../../Web/HttpResponse.h"
#include "Compiler.h"
#include "../../DB/DBContext.h"

#include <stdio.h>
#include <iostream>

USING_NS_DRAGON;
USING_NS_V8;
USING_NS_BOOST;

static Persistent<ObjectTemplate> CompilerTemplate;
static Persistent<ObjectTemplate> LibraryTempl;
static Persistent<ObjectTemplate> ExportTemplate;

static Handle<Value> LoadCallback(const Arguments& args);
static Handle<Value> LoadLibraryCallback(const Arguments& args);
static Handle<ObjectTemplate> MakeCompilerTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objT = ObjectTemplate::New();
	objT->SetInternalFieldCount(1);
	objT->Set(String::New("LoadLibrary"), FunctionTemplate::New(LoadLibraryCallback));

	return scope.Close(objT);
}

static Handle<ObjectTemplate>MakeLibraryTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objT = ObjectTemplate::New();
	objT->SetInternalFieldCount(1);
	objT->Set(String::New("Load"), FunctionTemplate::New(LoadCallback));

	return scope.Close(objT);
}

static Handle<ObjectTemplate>MakeExportTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objT = ObjectTemplate::New();
	return scope.Close(objT);
}

Handle<Object> JavaScriptCompiler::Wrap()
{
	HandleScope scope(isolate_);

	if (CompilerTemplate.IsEmpty()) {
		Handle<ObjectTemplate> temp = MakeCompilerTemplate(isolate_);
		CompilerTemplate.Reset(isolate_, temp);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate_, CompilerTemplate);
	Handle<Object> obj= objT->NewInstance();
	Handle<External> ptr = External::New(this);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

Handle<Object> JavaScriptCompiler::WrapLibrary()
{
	HandleScope scope(isolate_);

	if (LibraryTempl.IsEmpty()) {
		Handle<ObjectTemplate> templ = MakeLibraryTemplate(isolate_);
		LibraryTempl.Reset(isolate_, templ);
	}
	Handle<ObjectTemplate> objT =Local<ObjectTemplate>::New(isolate_, LibraryTempl);
	Handle<Object> obj= objT->NewInstance();
	Handle<External> ptr = External::New(&libs_);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}
LibraryMap *UnwrapLibrary(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<LibraryMap *>(ptr);
}
JavaScriptCompiler *UnwrapComplier(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<JavaScriptCompiler *>(ptr);
}

JavaScriptCompiler::JavaScriptCompiler()
{
	isolate_ = Isolate::GetCurrent();
}

Handle<Object>JavaScriptCompiler::Export(JavasScriptLibrary &library,
																		Handle<Function> func)
{
	HandleScope scope(isolate_);

	if (ExportTemplate.IsEmpty()) {
		Handle<ObjectTemplate> temp = MakeExportTemplate(isolate_);
		ExportTemplate.Reset(isolate_, temp);
	}
	Handle<ObjectTemplate> objT = Local<ObjectTemplate>::New(isolate_, ExportTemplate);
	Handle<Object> exports = objT->NewInstance();

	const int argc = 1;
	Handle<Context> ctx = Local<Context>::New(isolate_, library.ctx_); 
	Handle<Value> argv[argc] = { exports };
	Handle<Value> result_t = func->Call(ctx->Global(), argc, argv);
	Handle<Object> obj  = Handle<Object>::Cast(result_t);
	TryCatch try_catch;
	if (obj.IsEmpty()) {
			String::Utf8Value error(try_catch.Exception());
			printf(*error);
			exit(0);
	} 
	return scope.Close(exports);
}

Handle<String> JavaScriptCompiler::LoadJavaScriptSource(const std::string &path, bool isLib)
{
	const char *exports_start = "(function(exports) {";
	const char *exports_end  = "});";
	HandleScope scope(isolate_);

	FILE *f = fopen(path.c_str(), "rb");

	if (f == NULL) {
		lastError = kErrJSFileOpen;
		return Handle<String>();
	}

	fseek(f, 0, SEEK_END);
	size_t bytesOfFile = ftell(f);
	rewind(f);

	size_t total;
	if (isLib) {
		total = bytesOfFile + strlen(exports_start) + strlen(exports_end);
	} else {
		total = bytesOfFile;
	}

	char *buf = new char[total+1];
	buf[total] = '\0';

	if (isLib) {
		strncpy(buf, exports_start, strlen(exports_start));
		buf += strlen(exports_start);
		strncpy(buf + bytesOfFile, exports_end, strlen(exports_end));
	}

	size_t remaining = bytesOfFile;
	char *cp = buf;
	while (remaining)
	{
		size_t bytesOfRead = fread(cp, 1, bytesOfFile, f);
		remaining -= bytesOfRead;
		cp += bytesOfRead;
	}
	
	if (isLib) {
		buf -= strlen(exports_start);
	}

	Handle<String> source = String::New(buf, total);
	delete [] buf;
	fclose(f);

	return scope.Close(source);
}

void JavaScriptCompiler::Load(const std::string &AppPath)
{
	HandleScope scope(GetIsolate());
	const std::string path = AppPath + "Controllers/";

	filesystem::path ctrolsPath(path);
	filesystem::directory_iterator end_itr;
	for (filesystem::directory_iterator itr(ctrolsPath); itr != end_itr; ++itr)
	{
		if (itr->path().extension().string() == ".js")
		{
			JavasScriptSource jss;
			jss.path_				= path + itr->path().filename().string();
			jss.id_					= filesystem::change_extension(itr->path().filename(), "").string();
			jss.lastModified_	= last_write_time(itr->path());
			jss.ctx_.Reset(GetIsolate(), Context::New(GetIsolate()));

			Handle<String> source = LoadJavaScriptSource(jss.path_);

			Handle<Context> ctx = Local<Context>::New(isolate_, jss.ctx_);
			Context::Scope scope_ctx(ctx);
			if (ExecuteScript(source).IsEmpty()) {
				fprintf(stderr, "error in %s", jss.path_.c_str());
				exit(1);
			}
			controllers_[jss.id_] = jss;
		}
	}
}

void JavaScriptCompiler::LoadLibs(const std::string &AppPath)
{
	HandleScope scope(GetIsolate());
	const std::string path = AppPath + "Librarys/";
	{
		JavasScriptLibrary jss;
		jss.path_				= "";
		jss.id_					= "SqlContext";
		jss.lastModified_	= time(NULL);
		jss.ctx_.Reset(GetIsolate(), Context::New(GetIsolate())); 

		MySQLContext *ctx =  new MySQLContext();
		Handle<Context> v8ctx = Local<Context>::New(isolate_, jss.ctx_);
		Context::Scope scope_ctx(v8ctx);
		Handle<Object> ctx_obj = WrapMySQLContext(ctx, GetIsolate());
		jss.obj_.Reset(isolate_, ctx_obj);
		libs_[jss.id_] = jss;
	}

	filesystem::path ctrolsPath(path);
	filesystem::directory_iterator end_itr;
	for (filesystem::directory_iterator itr(ctrolsPath); itr != end_itr; ++itr)
	{
		if (itr->path().extension().string() == ".js")
		{
			JavasScriptLibrary jss;
			jss.path_				= path + itr->path().filename().string();
			jss.id_					= filesystem::change_extension(itr->path().filename(), "").string();
			jss.lastModified_	= last_write_time(itr->path());
			jss.ctx_.Reset(GetIsolate(), Context::New(GetIsolate()));

			Handle<String> source = LoadJavaScriptSource(jss.path_, true);

			Handle<Context> v8ctx = Local<Context>::New(isolate_, jss.ctx_);
			Context::Scope scope_ctx(v8ctx);
			Handle<Object> libraryObj = WrapLibrary();
			v8ctx->Global()->Set(String::New("Library"), libraryObj);

			Handle<Object> compilerObj = Wrap();
			v8ctx->Global()->Set(String::New("Compiler"), compilerObj);

			Handle<Value> result = ExecuteScript(source);
			if (result.IsEmpty()) {
				fprintf(stderr, "error in %s", jss.path_.c_str());
				exit(1);
			}

			Handle<Function> func = Handle<Function>::Cast(result);
			Handle<Object> exports = Export(jss, func);
			jss.obj_.Reset(isolate_, exports);
			libs_[jss.id_] = jss;
		}
	}
}

void JavaScriptCompiler::Reload(const std::string &id)
{
	JavasScriptSource &jss = controllers_[id];
	filesystem::path js_path(jss.path_);
	if (jss.lastModified_ != last_write_time(js_path)) {
		Handle<String> source = LoadJavaScriptSource(jss.path_);
		Handle<Context> ctx = Local<Context>::New(isolate_, jss.ctx_);
		Context::Scope scope_ctx(ctx);
		if (ExecuteScript(source).IsEmpty()) {
				fprintf(stderr, "Reload Script Error : %s", jss.path_.c_str());
				exit(1);
		}
	}
}

void JavaScriptCompiler::ReloadLib(const std::string &id)
{
	JavasScriptLibrary &jss = libs_[id];
	filesystem::path js_path(jss.path_);
	if (jss.lastModified_ != last_write_time(js_path)) {
		Handle<String> source = LoadJavaScriptSource(jss.path_, true);
		Handle<Context> ctx = Local<Context>::New(isolate_, jss.ctx_);
		Context::Scope scope_ctx(ctx);
		Handle<Value> result = ExecuteScript(source);
		if (result.IsEmpty()) {
			fprintf(stderr, "error in %s", jss.path_.c_str());
			exit(1);
		}

		Handle<Object> exports = Export(jss, Handle<Function>::Cast(result));
		jss.obj_.Reset(isolate_, exports);
	}
}

Handle<Value> JavaScriptCompiler::ExecuteScript(Handle<String> source)
{
		HandleScope scope(isolate_);
		TryCatch try_catch;
		Handle<Script> script = Script::Compile(source);
		if (script.IsEmpty()) {
			String::Utf8Value err(try_catch.Exception());
			fprintf(stderr, "v8 Compile error : %s.", *err);
			return Handle<Value>();
		}

		Handle<Value> result = script->Run();
		if (result.IsEmpty()) {
			String::Utf8Value err(try_catch.Exception());
			Handle<Message> msg = try_catch.Message();
			fprintf(stderr, "v8 Run error : %s.", *err);
			return Handle<Value>();
		}

		return scope.Close(result);;
}

static Handle<Value> LoadCallback(const Arguments& args)
{
	LibraryMap *libs = UnwrapLibrary(args.Holder());
	if (args.Length() != 1) {
		return Undefined();
	}
	Handle<Value> id = args[0];
	String::Utf8Value strId(id);

	LibraryMap::iterator iter = libs->find(*strId);
	if (iter == libs->end()) {
			return Undefined();
	}

	Handle<Object> obj = Local<Object>::New(args.GetIsolate(), iter->second.obj_);
	return obj;
}

static Handle<Value> LoadLibraryCallback(const Arguments& args)
{
	JavaScriptCompiler *c = UnwrapComplier(args.Holder());

	if (args.Length() != 1) {
		return Undefined();
	}
	Handle<Value> id = args[0];
	String::Utf8Value strId(id);

	c->ReloadLib(*strId);

	Handle<Object> obj = Local<Object>::New(args.GetIsolate(), c->GetLibraryMap()[*strId].obj_);
	return obj;
}