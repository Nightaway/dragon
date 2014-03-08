#include "UrlRouting.h"

NS_USING_DRAGON
NS_USING_V8

UrlRouting::UrlRouting(JavaScriptCompiler &jsCompiler) 
			: JavaScriptModule(jsCompiler), pSource_(NULL)
{
	
}

void UrlRouting::Init()
{
	pSource_ = jsCompiler_.GetSource()["UrlRouting"];
	if (pSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), pSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> initFuncValue = ctx->Global()->Get(String::NewSymbol("Init"));
		if (!initFuncValue.IsEmpty() && initFuncValue->IsFunction()) {
			Handle<Function> initFunc = Handle<Function>::Cast(initFuncValue);
			Handle<Value> ret = initFunc->Call(ctx->Global(), 0, NULL);
		}
	}
}

void UrlRouting::Dispose()
{
	if (pSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), pSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> initFuncValue = ctx->Global()->Get(String::NewSymbol("Dispose"));
		if (!initFuncValue.IsEmpty() && initFuncValue->IsFunction()) {
			Handle<Function> initFunc = Handle<Function>::Cast(initFuncValue);
			Handle<Value> ret = initFunc->Call(ctx->Global(), 0, NULL);
		}
	}
}

RoutingTable UrlRouting::Route(std::string Url)
{
	RoutingTable routingTable;
	if (pSource_ != NULL) {
		HandleScope scope(jsCompiler_.GetIsolate());
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), pSource_->ctx_);
		Context::Scope context_scope(ctx);
		Handle<Value> initFuncValue = ctx->Global()->Get(String::NewSymbol("Route"));
		if (!initFuncValue.IsEmpty() && initFuncValue->IsFunction()) {
			Handle<Function> func = Handle<Function>::Cast(initFuncValue);

			const int argc = 1;
			Handle<String> url = String::New(Url.c_str(), Url.length());
			
			Handle<Value> argv[argc] = {url};
			Handle<Value> ret  = func->Call(ctx->Global(), argc, argv);
			Handle<Object> obj = Handle<Object>::Cast(ret);

			Handle<Value> ctr_name_value = obj->Get(String::New("ControllerName"));
			String::AsciiValue asciiCtrName(ctr_name_value);
			Handle<Value> action_name_value = obj->Get(String::New("ActionName"));
			String::AsciiValue asciiActionName(action_name_value);
			Handle<Value> qs_name_value = obj->Get(String::New("QueryString"));
			String::AsciiValue asciiQS(qs_name_value);

			routingTable.ControllerName = *asciiCtrName;
			routingTable.ActionName     = *asciiActionName;
			routingTable.QueryString    = *asciiQS;
		}
	}
	return routingTable;
}
