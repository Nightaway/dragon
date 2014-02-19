#ifndef __URL_REWRITING_H__
#define __URL_REWRITING_H__

#include "../core/Macro.h"
#include "../core/Module.h"
#include "../core/StringRef.h"

NS_DRAGON

	class UrlRewriting
			: public JavaScriptModule {
		const static char *moduleName_;
		JavaScriptSource *moduleSource_;

	public:
		UrlRewriting(JavaScriptCompiler &jsCompiler);

		virtual void Init();
		virtual void Dispose();

		virtual std::string Rewrite(StringRef refUrl);
	};

NS_END

#endif
