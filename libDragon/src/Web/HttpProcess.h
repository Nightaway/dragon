#ifndef __HTTP_PROCESS_H__
#define __HTTP_PROCESS_H__

NS_DRAGON
	
	class JavaSciprtHttpProcess {
		JavaScriptCompiler &jsc_;

		v8::Handle<v8::ObjectTemplate> MakeHttpRquestTemplate();
		v8::Handle<v8::ObjectTemplate> MakeHttpResponseTemplate();
		static v8::Handle<v8::ObjectTemplate> MakeMapTemplate(v8::Isolate *isolate);

		static v8::Handle<v8::Value> GetProtocol(v8::Local<v8::String> name,
																			const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetMethod(v8::Local<v8::String> name,
																			const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetIP(v8::Local<v8::String> name,
																			const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetUrl(v8::Local<v8::String> name,
																	const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetUserAgent(v8::Local<v8::String> name,
																	const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetLang(v8::Local<v8::String> name,
																		const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetCookie(v8::Local<v8::String> name,
																		const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetQueryString(v8::Local<v8::String> name,
																					const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> GetStatusCode(v8::Local<v8::String> name,
																		const v8::AccessorInfo& info);
		static void SetStatusCode(v8::Local<v8::String> name,
												  v8::Local<v8::Value> value,
													const v8::AccessorInfo& info);
		static void SetContent(v8::Local<v8::String> name,
												  v8::Local<v8::Value> value,
													const v8::AccessorInfo& info);
		static void SetContentType(v8::Local<v8::String> name,
												     v8::Local<v8::Value> value,
													const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> SetCookie(const v8::Arguments& args);

		static v8::Persistent<v8::ObjectTemplate> requestTemplate;
		static v8::Persistent<v8::ObjectTemplate> responseTemplate;
		static v8::Persistent<v8::ObjectTemplate> mapTemplate;
		static v8::Persistent<v8::ObjectTemplate> mapmapTemplate;

		v8::Handle<v8::Object> WrapRequest(HttpRequest *request);
		static HttpRequest *UnwrapRequest(v8::Handle<v8::Object> obj);

		v8::Handle<v8::Object> WrapResponse(HttpResponse *request);
		static HttpResponse *UnwrapResponse(v8::Handle<v8::Object> obj);

		static v8::Handle<v8::Object> WrapMap(std::map<std::string, std::string> *map, v8::Isolate *isolate);
		static std::map<std::string, std::string> *UnwrapMap(v8::Handle<v8::Object> obj);

		//v8::Handle<v8::Object> WrapMapMap(HttpRequest *request);
		//static HttpRequest *UnwrapMapMap(v8::Handle<v8::Object> obj);

	public:
		JavaSciprtHttpProcess(JavaScriptCompiler &jsc) : jsc_(jsc) {}

		void process(HttpRequest &request, HttpResponse &response, 
							  const std::string &controllerName, const std::string &actionName, const std::string &parameters);
	};


NS_END


#endif