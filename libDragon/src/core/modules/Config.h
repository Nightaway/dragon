#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "../Macro.h"
#include "../StdType.h"
#include "../Module.h"

#include <map>
#include <string>

NS_DRAGON

	class Config : public JavaScriptModule {
			config_t config_;

		public:
			Config(JavaScriptCompiler &jsc);
			~Config();

			virtual void Init();
			virtual void Dispose();
			virtual v8::Handle<v8::Object> Wrap();

			void Parse(const char *path);
			key_value_t &operator[](const section_t &section) 
			{
				return config_[section];
			}
			config_t &GetConfig()
			{
				return config_;
			}
	};

NS_END

#endif
