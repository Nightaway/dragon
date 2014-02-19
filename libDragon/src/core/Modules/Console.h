#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "../Macro.h"
#include "../Module.h"

NS_DRAGON
	
	class Console :
			public JavaScriptModule {

		public:
			Console(JavaScriptCompiler &jsCompiler);
			~Console();

			virtual void Init();
			virtual void Dispose();

			void Log(std::string msg);

			v8::Handle<v8::Object> Wrap();
	};

NS_END

#endif
