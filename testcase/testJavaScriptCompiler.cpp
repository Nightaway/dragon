#include "config.h"

#include <gtest/gtest.h>
#include <dragon.h>
#include <iostream>

using namespace dragon;
using namespace v8;


void CallJSFunction(JavaScriptCompiler &jsc,  
		    JavaScriptSource *source,
		    const char *name)
{
	HandleScope scope(jsc.GetIsolate());
	Local<Context> ctx = Local<Context>::New(jsc.GetIsolate(), source->ctx_);
	Context::Scope context_scope(ctx);
	Handle<Value> rewriteFuncValue = ctx->Global()->Get(String::NewSymbol(name));
	if (!rewriteFuncValue.IsEmpty() && rewriteFuncValue->IsFunction()) {
		const int argc = 0;
		Handle<Function> initFunc = Handle<Function>::Cast(rewriteFuncValue);
		Handle<Value> ret = initFunc->Call(ctx->Global(), argc, NULL);
		String::AsciiValue ascii(ret);
		ASSERT_STREQ("OK", *ascii);
	}
}


TEST(JavaScriptCompiler, Load) {
	JavaScriptCompiler jsc;
	jsc.Load(DRAGON_TEST_PATH, "testJavaScriptCompiler/subdir");
        SourceMap &source= jsc.GetSource();
        JavaScriptSource *source1 = source["1"];

	HandleScope scope(jsc.GetIsolate());
	Local<Context> ctx = Local<Context>::New(jsc.GetIsolate(), source1->ctx_);
	Context::Scope context_scope(ctx);
	Handle<Value> rewriteFuncValue = ctx->Global()->Get(String::NewSymbol("test1"));
	if (!rewriteFuncValue.IsEmpty() && rewriteFuncValue->IsFunction()) {
		const int argc = 0;
		Handle<Function> initFunc = Handle<Function>::Cast(rewriteFuncValue);
		Handle<Value> ret = initFunc->Call(ctx->Global(), argc, NULL);
		String::AsciiValue ascii(ret);
		ASSERT_STREQ("OK1", *ascii);
	}
}
