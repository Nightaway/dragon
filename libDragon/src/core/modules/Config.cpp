#include "Config.h"

#include "../QueryString.h"
#include "../../wrapper/Wrapper.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using boost::property_tree::ini_parser::read_ini;
using boost::property_tree::ptree;
NS_USING_DRAGON
NS_USING_V8

Config::Config(JavaScriptCompiler &jsc) : JavaScriptModule(jsc)
{
	_name_ = "config";
	_v_major_ = 1;
	_v_minor_ = 0;
}

Config::~Config()
{

}


void Config::Init()
{
	jsCompiler_.AddModule(_name_, this);
}

void Config::Dispose()
{

}

void Config::Parse(const char *path)
{
	ptree ini;
	read_ini(path, ini);
	for (ptree::iterator iter=ini.begin(); iter!=ini.end(); ++iter)
	{
		std::map<std::string, std::string> nestmap;
		for (ptree::iterator child_iter=iter->second.begin(); child_iter!=iter->second.end(); ++child_iter)
		{
			nestmap[child_iter->first] = child_iter->second.data();
		}
		config_[iter->first] = nestmap;
	}
}

static Persistent<ObjectTemplate> configTemplate;
static Handle<ObjectTemplate> MakeConfigTemplate(Isolate *isolate);

Handle<Object> Config::Wrap()
{

}

void MapGet(Local<String> name,
            const PropertyCallbackInfo<Value>& info) {
  // Fetch the map wrapped by this object.
  Config* obj = Unwrap<Config>(info.Holder());
  config_t &config = obj->GetConfig();

  // Convert the JavaScript string to a std::string.
  std::string key = ObjectToString(name);

  // Look up the value if it exists using the standard STL ideom.
  config_t::iterator iter = config.find(key);

  // If the key is not present return an empty handle as signal
  if (iter == config.end()) return;

  Isolate *isolate = info.GetIsolate();

  // Otherwise fetch the value and wrap it in a JavaScript string
  key_value_t& value = (*iter).second;
  Handle<Object> ret = Wrapper::wrapKV(isolate, value);
  info.GetReturnValue().Set(ret);
}

void MapSet(Local<String> name,
            Local<Value> value_obj,
            const PropertyCallbackInfo<Value>& info) {

  using std::string;
  using std::map;

  // Fetch the map wrapped by this object.
  Config* obj = Unwrap<Config>(info.Holder());
  config_t &config = obj->GetConfig();

  // Convert the key and value to std::strings.
  string key = ObjectToString(name);
  key_value_t &kv = config[key];

  Isolate *isolate = info.GetIsolate();
  Handle<Object> ret = Wrapper::wrapKV(isolate, kv);
  // Return the value; any non-empty handle will work.
  info.GetReturnValue().Set(ret);
}

Handle<ObjectTemplate> MakeConfigTemplate(Isolate *isolate)
{
	HandleScope scope(isolate);
	Handle<ObjectTemplate> objTemplate = ObjectTemplate::New();
  	objTemplate->SetInternalFieldCount(1);
 	objTemplate->SetNamedPropertyHandler(MapGet, MapSet);
	return scope.Close(objTemplate);
}

