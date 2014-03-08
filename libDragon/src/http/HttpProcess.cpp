#include "HttpProcess.h"

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
	SourceMap sources = jsCompiler_.GetSource();
	BOOST_FOREACH(SourcePair_t p, sources)
	{
		std::string::size_type pos = p.first.find("Controller");
		if (pos != std::string::npos) {
			std::string id = p.first.substr(0, pos);
			Controllers_[id] = sources[p.first];
		}
	}
}

void HttpProcess::Dispose()
{

}

void HttpProcess::Process(RoutingTable &routingTable, 
	     		  HttpRequest  &req, 
            		  HttpResponse &res)
{
	if (ProcessByJavaScript(routingTable, req, res))
	{
		std::cout << "Process By JS" << std::endl;
		return ;
	} else if (ProcessByCXX(routingTable, req, res)) {
		std::cout << "Process By CXX" << std::endl;
		return ;
	} else {
		res.SetResponse(404, "text/html", "Not Found Any Controller.");
	}
}

bool HttpProcess::ProcessByJavaScript(RoutingTable &routingTable, 
			              HttpRequest  &req, 
                                      HttpResponse &res)
{
	std::string controllerName = routingTable.ControllerName;
	if (Controllers_.find(controllerName) != Controllers_.end())
	{
		HandleScope scope(jsCompiler_.GetIsolate());
		JavaScriptSource *jsSource = Controllers_[controllerName];
		Local<Context> ctx = Local<Context>::New(jsCompiler_.GetIsolate(), jsSource->ctx_);
	        Context::Scope scope_ctx(ctx);

		std::string seperator = "&";
		Handle<Value> configValue = ctx->Global()->Get(String::New("config"));
		if (!configValue->IsUndefined()) {
			Handle<Object> configObj  = Handle<Object>::Cast(configValue);
			Handle<Value>  actionAttribute = configObj->Get(String::New("actionSeperator"));
			Handle<Object> actionAttrObj = Handle<Object>::Cast(actionAttribute);
			Handle<Value>  seperatorValue = actionAttrObj->Get(String::New(routingTable.ActionName.c_str()));
		
			if (!seperatorValue->IsUndefined()) 
				seperator = ObjectToString(seperatorValue);
		}
			
	//	std::string seperator = ObjectToString(seperatorValue);
		std::cout << "seperator:" << seperator << std::endl;

		/*Handle<Array> names = actionAttrObj->GetPropertyNames();
		for (unsigned i=0; i<names->Length(); ++i)
		{
			String::Utf8Value strName(names->Get(i));
			std::cout << "i:" << *strName << std::endl;
			//String::Utf8Value strValue(bag->Get(names->Get(i)));
			//std::cout << *strValue << std::endl;
		}*/

		Handle<Object> reqObj = Wrapper::wrap(jsCompiler_.GetIsolate(), req);
		ctx->Global()->Set(String::New("request"), reqObj);
		Handle<Object> resObj = Wrapper::wrap(jsCompiler_.GetIsolate(), res);
		ctx->Global()->Set(String::New("response"), resObj);
		Handle<Value> actionValue = ctx->Global()->Get(String::New(routingTable.ActionName.c_str(), routingTable.ActionName.length()));
		Handle<Function> actionFunction = Handle<Function>::Cast(actionValue);

		QueryString qs;
		parse(routingTable.QueryString, qs, seperator);
		qs["raw_qs"] = routingTable.QueryString;

		//std::cout << "parameter:" << routingTable.Parameter << std::endl;

		Handle<Object> qsObj = Wrapper::wrap(jsCompiler_.GetIsolate(), qs);

		const int argc = 1;
		Handle<Value> argv[argc] = {qsObj};
		actionFunction->Call(ctx->Global(), argc, argv);

		//std::cout << "StatusCode:"  << res.GetStatusCode()  << std::endl;
		//std::cout << "ContentType:" << res.GetContentType() << std::endl;
		//std::cout << "Content:"     << res.GetContent()     << std::endl;

		return true;
	} 

	return false;
}

bool HttpProcess::ProcessByCXX(RoutingTable &routingTable,
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

					//std::cout << "actionTable->actName:" << actionTable->actName << std::endl;
					Action action = actionTable->action;
					const char *seperator = actionTable->separator;
					parse(routingTable.QueryString, qs, seperator);
					qs["raw_qs"] = routingTable.QueryString;
					std::cout << "seperator:" << seperator << std::endl;
					(pCtrl->*action)(qs);
					
					return true;
				}
			}
		}
	}

	return false;
}

