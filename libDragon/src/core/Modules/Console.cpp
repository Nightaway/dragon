#include "Console.h"
	
#include <iostream>

NS_USING_DRAGON
NS_USING_V8

Console::Console(JavaScriptCompiler &jsCompiler) : JavaScriptModule(jsCompiler)
{

}

Console::~Console()
{

}
			
void Console::Init()
{

}
			
void Console::Dispose()
{

}
		
void Console::Log(std::string msg)
{
	std::cout << msg;
}

static Persistent<ObjectTemplate> consoleTemplate;
static Handle<ObjectTemplate> MakeConsoleTemplate(Isolate *isolate);

static void LogCallback(const v8::FunctionCallbackInfo<Value>& args) 
{
	if (args.Length() !=1) return ;
	
	Handle<Value> msg_value = args[0];
	v8::String::Utf8Value msg_str(msg_value);

	std::cout << *msg_str;
}

Handle<Object> Console::Wrap()
{
	Isolate *isolate = jsCompiler_.GetIsolate();
	HandleScope scope(isolate);

	if (consoleTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeConsoleTemplate(isolate);
		consoleTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, consoleTemplate);
	Handle<Object> obj = local->NewInstance();
	return scope.Close(obj);
}

Handle<ObjectTemplate> MakeConsoleTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->Set(v8::String::New("log"), FunctionTemplate::New(LogCallback));
	return scope.Close(objTemplate);
}
