#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Macro.h"
#include "RuntimeInfo.h"
#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"

NS_DRAGON

class Application;
class Controller {
protected:
	DECLARE_CONTROLLER(Controller)

	static ClassTable *lastClassTable_;

protected:
	HttpRequest  *request;
	HttpResponse *response;
        Application  *app;
	
public:
	Controller() : request(NULL), response(NULL) {}
	~Controller() {}

	void SetHttpRequest(HttpRequest *req)
	{
	  request  = req;
	}

	void SetHttpResponse(HttpResponse *res)
	{
	  response = res;
	}

        void SetApplication(Application *application)
        {
          app = application;
        }
        
	virtual ActionTable *GetActionTable();
};

NS_END

#endif
