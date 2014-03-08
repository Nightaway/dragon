static Persistent<ObjectTemplate> keyValueTemplate;
static Handle<ObjectTemplate> MakekeyValueTemplate(Isolate *isolate);

Handle<Object> Wrapper::wrapKV(Isolate *isolate, key_value_t &kv)
{
	HandleScope scope(isolate);

	if (keyValueTemplate.IsEmpty()) {
		Handle<ObjectTemplate> objTemplate = MakekeyValueTemplate(isolate);
		keyValueTemplate.Reset(isolate, objTemplate);
	}
	Handle<ObjectTemplate> local = Local<ObjectTemplate>::New(isolate, keyValueTemplate);
	Handle<Object> obj = local->NewInstance();
	Handle<External> kv_ptr = External::New(&kv);
	obj->SetInternalField(0, kv_ptr);
	return scope.Close(obj);
}

static void MapGet(Local<String> name,
            	const PropertyCallbackInfo<Value>& info) {

  // Fetch the map wrapped by this object.
  key_value_t* obj = Unwrap<key_value_t>(info.Holder());

  // Convert the JavaScript string to a std::string.
  std::string key = ObjectToString(name);

  // Look up the value if it exists using the standard STL ideom.
  key_value_t::iterator iter = obj->find(key);

  // If the key is not present return an empty handle as signal
  if (iter == obj->end()) return;

  // Otherwise fetch the value and wrap it in a JavaScript string
  const value_t& value = (*iter).second;
  info.GetReturnValue().Set(
  String::New(value.c_str(), static_cast<int>(value.length())));
}

static void MapSet(Local<String> name,
            	   Local<Value> value_obj,
            	   const PropertyCallbackInfo<Value>& info) {

  using std::string;
  // Fetch the map wrapped by this object.
  key_value_t* obj = Unwrap<key_value_t>(info.Holder());

  // Convert the key and value to std::strings.
  string key = ObjectToString(name);
  string value = ObjectToString(value_obj);

  // Update the map.
  (*obj)[key] = value;

  // Return the value; any non-empty handle will work.
  info.GetReturnValue().Set(value_obj);
}

Handle<ObjectTemplate> MakekeyValueTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate =  ObjectTemplate::New();
	objTemplate->SetInternalFieldCount(1);
	objTemplate->SetNamedPropertyHandler(MapGet, MapSet);
	return scope.Close(objTemplate);
}
