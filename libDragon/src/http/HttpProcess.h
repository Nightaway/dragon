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

	class HttpProcess :
		public JavaScriptModule
 	{
		std::map<std::string, JavaScriptSource *> Controllers_;

		bool ProcessByJavaScript(RoutingTable &routingTable, 
					     HttpRequest  &req, 
                                             HttpResponse &res);
		bool ProcessByCXX(RoutingTable &routingTable, 
					     HttpRequest  &req, 
                                             HttpResponse &res);

		public:
			HttpProcess(JavaScriptCompiler &jsCompiler);
			~HttpProcess();

			virtual void Init();
			virtual void Dispose();

			virtual void Process(RoutingTable &routingTable, 
					     HttpRequest  &req, 
                                             HttpResponse &res);
	};

NS_END

#endif
