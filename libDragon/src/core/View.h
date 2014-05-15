#ifndef __VIEW_H__
#define __VIEW_H__

#include "Macro.h"
#include "DragonCore.h"
#include "QueryString.h"
#include "modules/Config.h"

#include <string>
#include <boost/unordered_map.hpp>

NS_DRAGON
	typedef boost::unordered_map<std::string, std::string> ViewBag;
	typedef boost::unordered_map<std::string, std::string> ViewCache;

	class View {
		std::string viewName;
		ViewBag viewBag;
	public:
		View(std::string name) : viewName(name)	{}

		std::string getViewName()
		{
			return viewName;
		}

		void setViewName(std::string viewName)
		{
			this->viewName = viewName;
		}

		ViewBag &getViewBag()
		{
			return viewBag;
		}

		void DataBind(QueryString &qs)
		{
			QueryString::iterator end = qs.end();
			for (QueryString::iterator iter=qs.begin(); iter != end; ++iter)
			{
				viewBag[iter->first] = qs[iter->first];
			}
		}

		void DataBind(Config &conf)
		{
			config_t &sections = conf.GetConfig();
			config_t::iterator end = sections.end();
			for (config_t::iterator iter = sections.begin(); iter != end; ++iter)
			{
				key_value_t kv = iter->second;
				key_value_t::iterator end = kv.end();
				for (key_value_t::iterator iter = kv.begin(); iter != end; ++iter)
				{
					viewBag[iter->first] = kv[iter->first];
				}
			}
		}
	};

	class ViewEngine
	{
		ViewCache viewCache;

	public:
		ViewEngine()  {}

		void Init();
		void Dispose();

		ViewCache &getViewCache()
		{
			return viewCache;
		}

		std::string Out(View &v, std::string &instance);
	};

	class ViewParser : 
				public Parser {
			CharStream s;
			ViewBag &viewBag;

			bool getNextVaribale(std::string &var);

	public:
		ViewParser(std::string str, ViewBag &bag) : s(str), viewBag(bag) {}
			
		std::string parse(char endCh);
	};

NS_END

#endif
