#ifndef __COMPILER_H__
#define __COMPILER_H__

NS_DRAGON 

	struct JavasScriptSource {
		std::string path_;
		std::string id_;
		time_t lastModified_;
		v8::Persistent<v8::Context> ctx_;
	};

	struct JavasScriptLibrary : 
						public JavasScriptSource {
		v8::Persistent<v8::Object>  obj_;
	};

	typedef std::map<std::string, JavasScriptSource> ControllerMap;
	typedef std::map<std::string, JavasScriptLibrary> LibraryMap;
	class JavaScriptCompiler {
		int lastError;
		ControllerMap controllers_;
		LibraryMap libs_;
		v8::Isolate *isolate_;

		v8::Handle<v8::String>  LoadJavaScriptSource(const std::string &path, bool isLib = false);
		v8::Handle<v8::Object> Export(JavasScriptLibrary &library, v8::Handle<v8::Function> func);

	public:
		JavaScriptCompiler();
		~JavaScriptCompiler() {}

		void Load(const std::string &AppPath);
		void LoadLibs(const std::string &AppPath);
		void Reload(const std::string &id);
		void ReloadLib(const std::string &id);
		v8::Handle<v8::Value> ExecuteScript(v8::Handle<v8::String>source);

		v8::Handle<v8::Object> WrapLibrary();
		v8::Handle<v8::Object> Wrap();

		//
		v8::Isolate *GetIsolate()
		{
			return isolate_;
		}

		ControllerMap &GetControllerMap()
		{
			return controllers_;
		}

		LibraryMap &GetLibraryMap()
		{
			return libs_;
		}
	};

NS_END


#endif