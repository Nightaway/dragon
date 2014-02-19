#include "UrlRewriting.h"

#include <iostream>
#include <cstring>

NS_USING_DRAGON
NS_USING_V8

const char *UrlRewriting::moduleName_ = "UrlRewriting";

UrlRewriting::UrlRewriting(JavaScriptCompiler &jsCompiler) 
			: JavaScriptModule(jsCompiler), moduleSource_(NULL)
{
	
}

void UrlRewriting::Init()
{
	moduleSource_ = jsCompiler_.GetSource()[moduleName_];
	if (moduleSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), moduleSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> initFuncValue = ctx->Global()->Get(String::NewSymbol("Init"));
		if (!initFuncValue.IsEmpty() && initFuncValue->IsFunction()) {
			Handle<Function> initFunc = Handle<Function>::Cast(initFuncValue);
			Handle<Value> ret = initFunc->Call(ctx->Global(), 0, NULL);
		}
	}
}

void UrlRewriting::Dispose()
{
	if (moduleSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), moduleSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> initFuncValue = ctx->Global()->Get(String::NewSymbol("Dispose"));
		if (!initFuncValue.IsEmpty() && initFuncValue->IsFunction()) {
			Handle<Function> initFunc = Handle<Function>::Cast(initFuncValue);
			Handle<Value> ret = initFunc->Call(ctx->Global(), 0, NULL);
		}
	}
}

std::string UrlRewriting::Rewrite(StringRef refUrl)
{
	if (moduleSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), moduleSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> rewriteFuncValue = ctx->Global()->Get(String::NewSymbol("Rewrite"));
		if (!rewriteFuncValue.IsEmpty() && rewriteFuncValue->IsFunction()) {
			const int argc = 1;
			Handle<String> url = String::New(refUrl.string(), refUrl.length());
			Handle<Value> argv[argc] = {url};
			Handle<Function> initFunc = Handle<Function>::Cast(rewriteFuncValue);
			Handle<Value> ret = initFunc->Call(ctx->Global(), argc, argv);
			String::AsciiValue ascii(ret);
			return std::string(*ascii, strlen(*ascii));
		}
	}
}
