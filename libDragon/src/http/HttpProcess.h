#ifndef __HTTP_PROCESS_H__
#define __HTTP_PROCESS_H__

#include "../core/Macro.h"
#include "../core/Module.h"

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "UrlRouting.h"

#include <string>
#include <map>

NS_DRAGON
  class Application;

	class HttpProcess :
		public JavaScriptModule
 	{
		bool ProcessByCXX(RoutingTable &routingTable,
                      Application &app,
				              HttpRequest  &req, 
                      HttpResponse &res);

		public:
			HttpProcess(JavaScriptCompiler &jsCompiler);
			~HttpProcess();

			virtual void Init();
			virtual void Dispose();

			void Process(RoutingTable &routingTable,
                   Application  &app,
					         HttpRequest  &req, 
                   HttpResponse &res);
	};

NS_END

#endif
