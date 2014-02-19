#include "Wrapper.h"

NS_USING_DRAGON
NS_USING_V8

static Persistent<ObjectTemplate> httpRequestTemplate;
static Handle<ObjectTemplate> MakeHttpRequestTemplate(Isolate *isolate);

static Persistent<ObjectTemplate> httpResponseTemplate;
static Handle<ObjectTemplate> MakeHttpResponseTemplate(Isolate *isolate);

static Persistent<ObjectTemplate> queryStringTemplate;
static Handle<ObjectTemplate> MakeQueryStringTemplate(Isolate *isolate);

void GetIp(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetHost(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetMethod(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetUrl(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetUserAgent(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetUserCookie(Local<String> name,
           const PropertyCallbackInfo<Value>& info);
void GetAcceptLang(Local<String> name,
       	   const PropertyCallbackInfo<Value>& info);

Handle<Object> Wrapper::wrap(Isolate *isolate, HttpRequest &req)
{
	HandleScope scope(isolate);

	if (httpRequestTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeHttpRequestTemplate(isolate);
		httpRequestTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, httpRequestTemplate);
	Handle<Object> obj = local->NewInstance();
	Handle<External> req_ptr = External::New(&req);
	obj->SetInternalField(0, req_ptr);
	return scope.Close(obj);
}

Handle<Object> Wrapper::wrap(Isolate *isolate, HttpResponse &res)
{
	HandleScope scope(isolate);

	if (httpResponseTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeHttpResponseTemplate(isolate);
		httpResponseTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, httpResponseTemplate);
	Handle<Object> obj = local->NewInstance();
	Handle<External> res_ptr = External::New(&res);
	obj->SetInternalField(0, res_ptr);
	return scope.Close(obj);
}

Handle<Object> Wrapper::wrap(Isolate *isolate, QueryString &qs)
{
	HandleScope scope(isolate);

	if (queryStringTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakeQueryStringTemplate(isolate);
		queryStringTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, queryStringTemplate);
	Handle<Object> obj = local->NewInstance();
	Handle<External> qs_ptr = External::New(&qs);
	obj->SetInternalField(0, qs_ptr);
	return scope.Close(obj);
}

Handle<ObjectTemplate> MakeHttpRequestTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);
	objTemplate->SetAccessor(String::NewSymbol("method"), GetMethod);
	objTemplate->SetAccessor(String::NewSymbol("url"), GetUrl);
	objTemplate->SetAccessor(String::NewSymbol("ip"), GetIp);
	objTemplate->SetAccessor(String::NewSymbol("host"), GetHost);
	objTemplate->SetAccessor(String::NewSymbol("agent"), GetUserAgent);
	objTemplate->SetAccessor(String::NewSymbol("cookie"), GetUserCookie);
	objTemplate->SetAccessor(String::NewSymbol("acceptLanguage"), GetAcceptLang);
	return scope.Close(objTemplate);
}

void GetUrl(Local<String> name,
           const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef Url = req->GetUrl();
	Handle<String> ret = String::New(Url.string(), static_cast<int>(Url.length()));
	info.GetReturnValue().Set(ret);
}

void GetIp(Local<String> name,
           const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef Ip = req->GetIp();
	Handle<String> ret = String::New(Ip.string(), static_cast<int>(Ip.length()));
	info.GetReturnValue().Set(ret);
}

void GetHost(Local<String> name,
           const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef Host = req->GetHost();
	Handle<String> ret = String::New(Host.string(), static_cast<int>(Host.length()));
	info.GetReturnValue().Set(ret);
}

void GetMethod(Local<String> name,
           const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	HttpMethod method = req->GetMethod();
	Handle<String> ret;
	switch (method) 
	{
		case kHttp_Method_Get:
		ret = String::New("Get");
		break;
		
		case kHttp_Method_Post:
		ret = String::New("Post");
		break;

		case kHttp_Method_Put:
		ret = String::New("Put");
		break;
		
		case kHttp_Method_Delete:
		ret = String::New("Delete");
		break;
	};
	
	info.GetReturnValue().Set(ret);
}

void GetUserAgent(Local<String> name,
        const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef agent = req->GetUserAgent();
	Handle<String> ret = String::New(agent.string(), static_cast<int>(agent.length()));
	info.GetReturnValue().Set(ret);
}

void GetUserCookie(Local<String> name,
        const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef cookie = req->GetUserCookie();
	Handle<String> ret = String::New(cookie.string(), static_cast<int>(cookie.length()));
	info.GetReturnValue().Set(ret);
}

void GetAcceptLang(Local<String> name,
        const PropertyCallbackInfo<Value>& info)
{
	HttpRequest* req = Unwrap<HttpRequest>(info.Holder());
	StringRef lang = req->GetAcceptLanguage();
	Handle<String> ret = String::New(lang.string(), static_cast<int>(lang.length()));
	info.GetReturnValue().Set(ret);
}

void SetStatusCode(Local<String> property,
    Local<Value> value,
    const PropertyCallbackInfo<void>& info);

void SetContentType(Local<String> property,
    Local<Value> value,
    const PropertyCallbackInfo<void>& info);
	
void SetContent( Local<String> property,
    Local<Value> value,
    const PropertyCallbackInfo<void>& info);

void SetStringResult(const v8::FunctionCallbackInfo<Value>& args);
void SetRedirectResult(const v8::FunctionCallbackInfo<Value>& args);
void SetDataRefResult(const v8::FunctionCallbackInfo<Value>& args);

Handle<ObjectTemplate> MakeHttpResponseTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);
	objTemplate->SetAccessor(String::New("status_code"), NULL, SetStatusCode);
	objTemplate->SetAccessor(String::New("content_type"), NULL, SetContentType);
	objTemplate->SetAccessor(String::New("content"), NULL, SetContent);
	objTemplate->Set(String::New("StringResult"), FunctionTemplate::New(SetStringResult));
	objTemplate->Set(String::New("RedirectResult"), FunctionTemplate::New(SetRedirectResult));
	return scope.Close(objTemplate);
}

void SetStringResult(const v8::FunctionCallbackInfo<Value>& args) 
{  
	if (args.Length() != 1) return ;

	Handle<Value> arg0 = args[0];
	if (!arg0->IsString()) return ;

	v8::String::Utf8Value string(arg0);
	HttpResponse *response = Unwrap<HttpResponse>(args.Holder());
	response->StringResult(*string);
}

void SetRedirectResult(const v8::FunctionCallbackInfo<Value>& args) 
{
	if (args.Length() != 1) return ;

	Handle<Value> arg0 = args[0];
	if (!arg0->IsString()) return ;

	v8::String::Utf8Value url(arg0);
	HttpResponse *response = Unwrap<HttpResponse>(args.Holder());
	response->RedirectResult(*url);
}

void SetStatusCode(Local<String> property,
    		   Local<Value> value,
    		   const PropertyCallbackInfo<void>& info)
{
        HttpResponse *response = Unwrap<HttpResponse>(info.Holder());
        int32_t status_code = value->Int32Value();
        response->SetStatusCode(status_code);
}

void SetContentType(Local<String> property,
    		    Local<Value> value,
    		    const PropertyCallbackInfo<void>& info)
{
        HttpResponse *response = Unwrap<HttpResponse>(info.Holder());
        String::Utf8Value content_type(value);
        response->SetContentType(*content_type);
}

void SetContent(Local<String> property,
                Local<Value> value,
                const PropertyCallbackInfo<void>& info)
{
        HttpResponse *response = Unwrap<HttpResponse>(info.Holder());
        String::Utf8Value content(value);
        response->SetContent(*content);
}

void GetQueryString(Local<String> name,
            const PropertyCallbackInfo<Value>& info);

void SetQueryString(Local<String> name,
            Local<Value> valueObj,
            const PropertyCallbackInfo<Value>& info);

Handle<ObjectTemplate> MakeQueryStringTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);
	objTemplate->SetNamedPropertyHandler(GetQueryString, SetQueryString);
	return scope.Close(objTemplate);
}

void GetQueryString(Local<String> name,
            	    const PropertyCallbackInfo<Value>& info)
{
	QueryString *qs = Unwrap<QueryString>(info.Holder());

	std::string key = ObjectToString(name);

	QueryString::iterator iter = qs->find(key);
	if (iter == qs->end()) {
		return ;
	}

	const std::string &value =  iter->second;
	Handle<String> ret = String::New(value.c_str(), static_cast<int>(value.length()));
	info.GetReturnValue().Set(ret);
}

void SetQueryString(Local<String> name,
            	    Local<Value> valueObj,
                    const PropertyCallbackInfo<Value>& info)
{
	QueryString *qs = Unwrap<QueryString>(info.Holder());

	std::string key   = ObjectToString(name);
	std::string value = ObjectToString(valueObj); 

	(*qs)[key] = value;
	info.GetReturnValue().Set(valueObj);
}
