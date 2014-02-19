#include "JSCompiler.h"

#include <cstring>
#include <cstdio>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include "Modules/Console.h"

NS_USING_DRAGON
NS_USING_V8
NS_USING_BOOST

JavaScriptCompiler::JavaScriptCompiler()
{
	isolate_ = Isolate::GetCurrent();
}

Handle<String> JavaScriptCompiler::LoadJavaScriptSource(const std::string &path, bool isLib)
{
        const char *exports_start = "(function(exports) {";
        const char *exports_end  = "});";
        HandleScope scope(isolate_);

        FILE *file = fopen(path.c_str(), "rb");
        if (file == NULL) {
                return Handle<String>();
        }

        fseek(file, 0, SEEK_END);
        size_t bytesOfFile = ftell(file);
        rewind(file);

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
                size_t bytesOfRead = fread(cp, 1, bytesOfFile, file);
                remaining -= bytesOfRead;
                cp += bytesOfRead;
        }
        
        if (isLib) {
                buf -= strlen(exports_start);
        }

        Handle<String> source = String::New( buf, total);
        delete [] buf;
        fclose(file);
	
        return scope.Close(source);
}

void JavaScriptCompiler::Load(const std::string &AppPath, const std::string &prefix)
{
        HandleScope scope(GetIsolate());
        const std::string path = AppPath + prefix + "/";

        filesystem::path ctrolsPath(path);
        filesystem::directory_iterator end_itr;
        for (filesystem::directory_iterator itr(ctrolsPath); itr != end_itr; ++itr)
        {
                if (itr->path().extension().string() == ".js")
                {
                       JavaScriptSource *jss = new JavaScriptSource();
                       jss->path_         = path + itr->path().filename().string();
                       jss->id_           = filesystem::change_extension(itr->path().filename(), "").string();
                       jss->lastModified_ = last_write_time(itr->path());
                       jss->ctx_.Reset(GetIsolate(), Context::New(GetIsolate()));

                       Handle<String> source = LoadJavaScriptSource(jss->path_);

                       Handle<Context> ctx = Local<Context>::New(isolate_, jss->ctx_);
			
		       static Console *console = NULL;
		       if (console == NULL)
				console = new Console(*this);

		       Context::Scope scope_ctx(ctx);
		       Handle<Object> consoleObj = console->Wrap();
		       ctx->Global()->Set(String::New("console"), consoleObj);

                       if (ExecuteScript(source).IsEmpty()) {
                               fprintf(stderr, "error in %s", jss->path_.c_str());
			       exit(1);
                       }
                       sources_[jss->id_] = jss;
                }
        }
}

Handle<Value> JavaScriptCompiler::ExecuteScript(Handle<String> source)
{
	HandleScope scope(isolate_);
        TryCatch try_catch;
        Local<Script> script = Script::Compile(source);
        if (script.IsEmpty()) {
            String::Utf8Value err(try_catch.Exception());
            fprintf(stderr, "v8 Compile error : %s.", *err);
            return Handle<Value>();
         }

         Local<Value> result = script->Run();
         if (result.IsEmpty()) {
             String::Utf8Value err(try_catch.Exception());
             Handle<Message> msg = try_catch.Message();
             fprintf(stderr, "v8 Run error : %s.", *err);
             return Handle<Value>();
         }
         return scope.Close(result);;
}
