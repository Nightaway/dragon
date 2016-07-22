#include "HttpProcess.h"

#include "../core/Application.h"
#include "../core/QueryString.h"
#include "../core/Controller.h"
#include "../wrapper/Wrapper.h"

#include <boost/foreach.hpp>

#include <iostream>

NS_USING_DRAGON
NS_USING_V8

HttpProcess::HttpProcess(JavaScriptCompiler &jsCompiler) :
	     JavaScriptModule(jsCompiler)
{

}

HttpProcess::~HttpProcess()
{

}

void HttpProcess::Init()
{

}

void HttpProcess::Dispose()
{

}

void HttpProcess::Process(RoutingTable &routingTable,
                          Application  &app,
	     		                HttpRequest  &req,
            		          HttpResponse &res)
{
	if (ProcessByCXX(routingTable, app, req, res)) {
		return ;
	} else {
		res.SetResponse(404, "text/html", "Not Found Any Controller.");
	}
}

bool HttpProcess::ProcessByCXX(RoutingTable &routingTable,
                               Application  &app,
	                             HttpRequest  &req, 
                               HttpResponse &res)
{
	Controller *pCtrl;
	QueryString qs;
	CLASS_FOREACH(clsTableRoot)
	{
		//std::cout << "clsInfo:" << clsTableRoot->pClsInfo->clsName_ << std::endl;
		std::string ControllerName = routingTable.ControllerName + "Controller";
		if (clsTableRoot->pClsInfo->clsName_ == ControllerName)
		{
			pCtrl = (*clsTableRoot->pClsInfo->createHandler_)();
			ACTION_FOREACH(pCtrl, actionTable)
			{
				if (routingTable.ActionName == actionTable->actName)
				{
					pCtrl->SetHttpRequest(&req);
					pCtrl->SetHttpResponse(&res);
					pCtrl->SetConfig(&app.GetConfig());
          pCtrl->SetApplication(&app);
	
					//std::cout << "actionTable->actName:" << actionTable->actName << std::endl;
					Action action = actionTable->action;
					const char *seperator = actionTable->separator;
					parse(routingTable.QueryString, qs, seperator);
					qs["raw_qs"] = routingTable.QueryString;

					//std::cout << "seperator:" << seperator << std::endl;
					(pCtrl->*action)(qs);
					
					return true;
				}
			}
		}
	}

	return false;
}

