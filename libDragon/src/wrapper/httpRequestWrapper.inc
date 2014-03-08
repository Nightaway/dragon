
static Persistent<ObjectTemplate> httpRequestTemplate;
static Handle<ObjectTemplate> MakeHttpRequestTemplate(Isolate *isolate);

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
