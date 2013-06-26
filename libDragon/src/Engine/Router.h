/*
 *				作者: 杨健
 *				日期: 2012-12-18
 *				功能: URL路由
 *				描述:	 解析HttpRequest传入的URL创建相应的Controller，然后调用相应的Action方法。
 */
#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <string>

NS_DRAGON 
		class Application;

		typedef struct 
		{
			std::string ControllerName;
			std::string ActionName;
			std::string Parameters;
			std::string OptionParameter;
		} routing_table_t;

		class Router
		{
		private:
			Application *app_;
			static std::map<std::string, std::string> routingMap;

			bool buildRoutingTable(routing_table_t *rt, std::string url);

		public:
			Router()  : app_(NULL)  {}
			~Router() {}

			static void Init();

			Application *getApp()
			{
				return app_;
			}

			void setApp(Application *app)
			{
				app_ = app;
			}

			void route(HttpRequest &request, 
							  HttpResponse &response);
		};

NS_END

#endif
