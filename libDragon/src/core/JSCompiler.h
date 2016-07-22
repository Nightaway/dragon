#ifndef __JS_COMPILER_H__
#define __JS_COMPILER_H__

#include "../config.h"
#include "Macro.h"

#include <v8.h>
#include <string>
#include <map>

NS_DRAGON
       class JavaScriptModule;

       struct JavaScriptSource {
                std::string path_;
                std::string id_;
                time_t lastModified_;
                v8::Persistent<v8::Context> ctx_;
       };

       struct JavaScriptLibrary : 
                                   public JavaScriptSource {
                v8::Persistent<v8::Object>  obj_;
       };

       typedef std::pair<std::string, JavaScriptSource *> SourcePair_t;
       typedef std::map<std::string, JavaScriptSource *>  SourceMap;
       typedef std::map<std::string, JavaScriptLibrary *> LibraryMap;
       typedef std::pair<std::string, JavaScriptModule *> ModulePair_t;
       typedef std::map<std::string, JavaScriptModule *>  ModuleMap;	

       class JavaScriptCompiler {
                SourceMap sources_;
                LibraryMap librarys_;
		            ModuleMap  modules_;

                v8::Isolate *isolate_;
                v8::Handle<v8::String>  LoadJavaScriptSource(const std::string &path, bool isLib = false);
                v8::Handle<v8::Object>  Export(JavaScriptLibrary &library, v8::Handle<v8::Function> func);

        public:
                JavaScriptCompiler();
                ~JavaScriptCompiler() {}

                void Load(const std::string &AppPath, const std::string &prefix);
                void Reload(const std::string &id);
                void LoadLibrary(const std::string &AppPath);

		void AddModule(const std::string name, JavaScriptModule *);

                v8::Handle<v8::Value> ExecuteScript(v8::Handle<v8::String>source);

                //v8::Handle<v8::Object> WrapLibrary();
                v8::Handle<v8::Object> Wrap();

                v8::Isolate *GetIsolate()
                {
                        return isolate_;
                }

                SourceMap &GetSource()
                {
                        return sources_;
                }

                ModuleMap &GetModuleMap()
                {
                        return modules_;
                }

                LibraryMap &GetLibrary()
                {
                        return librarys_;
                }
        };

NS_END


#endif
