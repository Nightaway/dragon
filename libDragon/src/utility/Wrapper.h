#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include "../core/Macro.h"
#include "../core/QueryString.h"
#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"

#include <v8.h>

NS_DRAGON

template <typename T>
T *Unwrap(v8::Handle<v8::Object> obj, int index = 0)
{
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(index));
	void *ptr = field->Value();
	return static_cast<T *>(ptr);
}

inline std::string ObjectToString(v8::Handle<v8::Value> v)
{
	v8::String::Utf8Value utf8_str(v);
	return std::string(*utf8_str);
}

class Wrapper {
public:
	static v8::Handle<v8::Object> wrap(v8::Isolate *isolate, HttpRequest  &req);
	static v8::Handle<v8::Object> wrap(v8::Isolate *isolate, HttpResponse &res);
	static v8::Handle<v8::Object> wrap(v8::Isolate *isolate, QueryString &qs);
};

NS_END

#endif
