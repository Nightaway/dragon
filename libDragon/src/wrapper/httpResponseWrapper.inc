
static Persistent<ObjectTemplate> httpResponseTemplate;
static Handle<ObjectTemplate> MakeHttpResponseTemplate(Isolate *isolate);

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

