#include "../Macro.h"

#include <v8.h>
#include <iostream>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../Engine/Internals/Compiler.h"
#include "HttpProcess.h"

#include "../Engine/View.h"

USING_NS_DRAGON;
USING_NS_V8;

Persistent<ObjectTemplate> JavaSciprtHttpProcess::requestTemplate;
Persistent<ObjectTemplate> JavaSciprtHttpProcess::responseTemplate;
Persistent<ObjectTemplate> JavaSciprtHttpProcess::mapTemplate;
Persistent<ObjectTemplate> JavaSciprtHttpProcess::mapmapTemplate;

Handle<Object> JavaSciprtHttpProcess::WrapRequest(HttpRequest *request)
{
	HandleScope scope(jsc_.GetIsolate());

	if (requestTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeHttpRquestTemplate();
		requestTemplate.Reset(jsc_.GetIsolate(), objTemplate);
	}
	Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(jsc_.GetIsolate(), requestTemplate);
	Handle<Object> obj = templ->NewInstance();
	Handle<External>ptr = External::New(request);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

HttpRequest *JavaSciprtHttpProcess::UnwrapRequest(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<HttpRequest *>(ptr);
}

Handle<Object> JavaSciprtHttpProcess::WrapResponse(HttpResponse *response)
{
	HandleScope scope(jsc_.GetIsolate());

	if (responseTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeHttpResponseTemplate();
		responseTemplate.Reset(jsc_.GetIsolate(), objTemplate);
	}
	Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(jsc_.GetIsolate(), responseTemplate);
	Handle<Object> obj = templ->NewInstance();
	Handle<External>ptr = External::New(response);
	obj->SetInternalField(0, ptr);

	return scope.Close(obj);
}

HttpResponse *JavaSciprtHttpProcess::UnwrapResponse(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<HttpResponse *>(ptr);
}

Handle<Value> JavaSciprtHttpProcess::GetProtocol(Local<String> name,
																	 const AccessorInfo& info) {
  // Extract the C++ request object from the JavaScript wrapper.
  HttpRequest* request = UnwrapRequest(info.Holder());

  // Fetch the Protocol.
  std::string proto;
  switch (request->getProtocol()) 
  {
  case HTTP:
	  proto = "HTTP";
	break;
  case HTTPS:
	  proto = "HTTPS";
  }

  // Wrap the result in a JavaScript string and return it.
  return String::New(proto.c_str(), static_cast<int>(proto.length()));
}

Handle<Value> JavaSciprtHttpProcess::GetMethod(Local<v8::String> name,
																				const AccessorInfo& info)
{
	 // Extract the C++ request object from the JavaScript wrapper.
	HttpRequest* request = UnwrapRequest(info.Holder());

	std::string method;
	switch(request->getMethod())
	{
	case GET:
		method = "GET";
		break;

	case POST:
		method = "POST";
		break;
	}

	return String::New(method.c_str(), static_cast<int>(method.length()));
}

Handle<Value> JavaSciprtHttpProcess::GetIP(Local<String> name,
																const AccessorInfo& info)
{
		HttpRequest* request = UnwrapRequest(info.Holder());
		std::string ip = request->getIP();
		return String::New(ip.c_str(), static_cast<int>(ip.length()));
}

Handle<v8::Value> JavaSciprtHttpProcess::GetUrl(Local<String> name,
																	const AccessorInfo& info)
{
	HttpRequest* request = UnwrapRequest(info.Holder());
	std::string url = request->getRoutingUrl();
	return String::New(url.c_str(), static_cast<int>(url.length()));
}

Handle<Value> JavaSciprtHttpProcess::GetUserAgent(Local<String> name,
																			const AccessorInfo& info)
{
	HttpRequest* request = UnwrapRequest(info.Holder());
	std::string userAgent = request->getUserAgent();
	return String::New(userAgent.c_str(), static_cast<int>(userAgent.length()));
}

Handle<Value> JavaSciprtHttpProcess::GetLang(Local<String> name,
																			   const AccessorInfo& info)
{
	HttpRequest* request = UnwrapRequest(info.Holder());
	std::string lang = request->getAcceptLanguage();
	return String::New(lang.c_str(), static_cast<int>(lang.length()));
}

Handle<Value> JavaSciprtHttpProcess::GetCookie(Local<String> name,
																				   const AccessorInfo& info)
{
	HttpRequest* request = UnwrapRequest(info.Holder());
	std::map<std::string, std::string> &cookie =request->getCookie();
	Handle<Object> obj = WrapMap(&cookie, info.GetIsolate());
	return obj;
}

Handle<Value> JavaSciprtHttpProcess::GetQueryString(Local<String> name,
																						   const AccessorInfo& info)
{
	HttpRequest* request = UnwrapRequest(info.Holder());
	std::map<std::string, std::string> &queryString =request->getQueryString();
	Handle<Object> obj = WrapMap(&queryString, info.GetIsolate());
	return obj;
}

void JavaSciprtHttpProcess::SetStatusCode(Local<String> name,
																  Local<Value> value,
															const AccessorInfo& info)
{
	HttpResponse *response = UnwrapResponse(info.Holder());
	int32_t status_code = value->Int32Value();
	response->setStatusCode(status_code);
}

void JavaSciprtHttpProcess::SetContent(Local<v8::String> name,
														    Local<v8::Value> value,
															const AccessorInfo& info)
{
	HttpResponse *response = UnwrapResponse(info.Holder());
	String::Utf8Value content(value);
	response->setContent(*content);
}

void JavaSciprtHttpProcess::SetContentType(Local<String> name,
															   Local<Value> value,
																const AccessorInfo& info)
{
	HttpResponse *response = UnwrapResponse(info.Holder());
	String::Utf8Value content_type(value);
	response->setContentType(*content_type);
}

Handle<Value> JavaSciprtHttpProcess::SetCookie(const Arguments& args) 
{
	HttpResponse *response = UnwrapResponse(args.Holder());
	if (args.Length() != 5)
		return Undefined();
	Handle<Value> key = args[0];
	Handle<Value> value = args[1];
	Handle<Value> expire = args[2];
	Handle<Value> path = args[3];
	Handle<Value> domain = args[4];
	if (!key->IsString() || !value->IsString() || !expire->IsString() || !path->IsString() || !domain->IsString()) {
		return Undefined();
	}
	
	String::Utf8Value strKey(key);
	String::Utf8Value strValue(value);
	String::Utf8Value strExpire(expire);
	String::Utf8Value strPath(path);
	String::Utf8Value strDomain(domain);
	response->setCookie(*strKey, *strValue, *strExpire, *strPath, *strDomain);

	return Undefined();
}

std::string ObjectToString(Handle<Value> v)
{
	String::Utf8Value utf8_str(v);
	return std::string(*utf8_str);
}

Handle<Object> JavaSciprtHttpProcess::WrapMap(std::map<std::string, std::string> *map, Isolate *isolate)
{
	HandleScope scope(isolate);

	if (mapTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objT= MakeMapTemplate(isolate);
		mapTemplate.Reset(isolate ,objT);
	}
	Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, mapTemplate);
	Handle<Object> obj = templ->NewInstance();
	Handle<External> map_ptr = External::New(map);
	obj->SetInternalField(0, map_ptr);

	return scope.Close(obj);
}

std::map<std::string, std::string> *UnwrapMap(Handle<Object> obj)
{
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void *ptr = field->Value();
	return static_cast<std::map<std::string, std::string> *>(ptr);
}

Handle<Value> GetMap(Local<String> name,
										 const AccessorInfo &info)
{
	std::map<std::string, std::string> *ptr = UnwrapMap(info.Holder());

	std::string key = ObjectToString(name);

	std::map<std::string, std::string>::iterator iter = ptr->find(key);
	if (iter == ptr->end()) {
		return Handle<Value>();
	}

	const std::string &value =  iter->second;
	return String::New(value.c_str(), static_cast<int>(value.length()));
}

Handle<Value> SetMap(Local<String> name,
										 Local<Value> v,
										 const AccessorInfo &info)
{
	std::map<std::string, std::string> *ptr = UnwrapMap(info.Holder());

	std::string key    = ObjectToString(name);
	std::string value = ObjectToString(v); 

	(*ptr)[key] = value;
	return v;
}

Handle<ObjectTemplate> JavaSciprtHttpProcess::MakeMapTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objT = ObjectTemplate::New();
	objT->SetInternalFieldCount(1);
	objT->SetNamedPropertyHandler(GetMap, SetMap);
	return scope.Close(objT);
}

Handle<ObjectTemplate> JavaSciprtHttpProcess::MakeHttpRquestTemplate()
{
	HandleScope scope(jsc_.GetIsolate());
	Handle<ObjectTemplate> objTemplate = ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);

	objTemplate->SetAccessor(String::NewSymbol("protocol"), GetProtocol);
	objTemplate->SetAccessor(String::NewSymbol("method"), GetMethod);
	objTemplate->SetAccessor(String::NewSymbol("ip"), GetIP);
	objTemplate->SetAccessor(String::NewSymbol("path"), GetUrl);
	objTemplate->SetAccessor(String::NewSymbol("agent"), GetUserAgent);
	objTemplate->SetAccessor(String::NewSymbol("lang"), GetLang);
	objTemplate->SetAccessor(String::NewSymbol("cookie"), GetCookie);
	objTemplate->SetAccessor(String::NewSymbol("query"), GetQueryString);

	return scope.Close(objTemplate);
}

Handle<ObjectTemplate> JavaSciprtHttpProcess::MakeHttpResponseTemplate()
{
	HandleScope scope(jsc_.GetIsolate());
	Handle<ObjectTemplate> objT = ObjectTemplate::New();
	objT->SetInternalFieldCount(1);

	objT->SetAccessor(String::NewSymbol("status_code"), NULL, SetStatusCode);
	objT->SetAccessor(String::NewSymbol("content"), NULL, SetContent);
	objT->SetAccessor(String::NewSymbol("content_type"), NULL, SetContentType);
	objT->Set(String::New("SetCookie"), FunctionTemplate::New(SetCookie));

	return scope.Close(objT);
}

void JavaSciprtHttpProcess::process(HttpRequest &request, 
														   HttpResponse &response,
														   const std::string &ControllerName,
														   const std::string &ActionName,
														   const std::string &parameters)
{
	JavaScriptCompiler &jsc = jsc_;
	HandleScope scope(jsc.GetIsolate());
	ControllerMap::iterator end = jsc.GetControllerMap().end();
	ControllerMap::iterator iter  = jsc.GetControllerMap().find(ControllerName);
	if (iter == end) {
		std::string errMsg = "Not Found Any Controller Name : ";
		errMsg += ControllerName;
		response.setStatusCode(404);
		response.setContentType("text/html");
		response.setContent(errMsg);
		return ;
	}
	
	JavasScriptSource &source = iter->second;
	jsc.Reload(source.id_);

	Handle<String> act_name = String::New(ActionName.c_str());
	Handle<Context> ctx = Local<Context>::New(jsc.GetIsolate(), source.ctx_);
	Context::Scope scope_ctx(ctx);
	Handle<Value> act = ctx->Global()->Get(act_name);

	if (act->IsUndefined()) {
		std::string errMsg = "Not Found Any Action Name : ";
		errMsg += ActionName;
		response.setStatusCode(404);
		response.setContentType("text/html");
		response.setContent(errMsg);
		return ;
	}

	if (!act->IsFunction()) {
		std::string errMsg = "Action";
		errMsg += ActionName;
		errMsg += " is not a function.";
		response.setStatusCode(404);
		response.setContentType("text/html");
		response.setContent(errMsg);
		return ;
	}

	request.parseQueryString(parameters);

	TryCatch try_catch;
	Handle<Function> act_func = Handle<Function>::Cast(act);
	
	Handle<Object> lib_obj = jsc.WrapLibrary();
	ctx->Global()->Set(String::NewSymbol("Library"), lib_obj);
	Handle<Object> compiler_obj = jsc.Wrap();
	ctx->Global()->Set(String::New("Compiler"), compiler_obj);

	const int argc = 2;
	Handle<Object> request_obj = WrapRequest(&request);
	Handle<Object> response_obj = WrapResponse(&response);
	Handle<Value> argv[argc] = { request_obj, response_obj };
	Handle<Value> result = act_func->Call(ctx->Global(), argc, argv);
	 
	if (!result.IsEmpty()) {
		if (result->IsObject()) {
			Handle<Object> obj = Handle<Object>::Cast(result);
			Handle<Value> r_id = obj->Get(String::New("id"));
			String::Utf8Value r_instance(obj->Get(String::New("instance")));

			//std::cout << *r_instance << std::endl;

			int64_t id = r_id->IntegerValue();
			//std::cout << id << std::endl;
			
			ViewBag vbag;
			Handle<Object> bag= Handle<Object>::Cast(obj->Get(String::New("bag")));
			Handle<Array> names = bag->GetPropertyNames();
			for (unsigned i=0; i<names->Length(); ++i)
			{
				String::Utf8Value strName(names->Get(i));
		//		std::cout << *strName << std::endl;
				String::Utf8Value strValue(bag->Get(names->Get(i)));
		//		std::cout << *strValue << std::endl;
				vbag[*strName] = *strValue;
			}
			ViewParser parse(*r_instance, vbag);
			std::string  cnt = parse.parse('$');

			response.setStatusCode(200);
			response.setContentType("text/html");
			response.setContent(cnt);
			return ;
		}
	}

	if (try_catch.HasCaught()) {
		String::Utf8Value error(try_catch.Exception());
		std::string errMsg = "Action Call Error : ";
		errMsg += *error;
		response.setStatusCode(404);
		response.setContentType("text/html");
		response.setContent(errMsg);
		return ;
	}
}