
static Persistent<ObjectTemplate> queryStringTemplate;
static Handle<ObjectTemplate> MakeQueryStringTemplate(Isolate *isolate);

void GetQueryString(Local<String> name,
            const PropertyCallbackInfo<Value>& info);

void SetQueryString(Local<String> name,
            Local<Value> valueObj,
            const PropertyCallbackInfo<Value>& info);

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

