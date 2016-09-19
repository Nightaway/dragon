#include "collectionController.h"

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../config.h"
#include "../Utility/rapidjson/document.h"

NS_USING_DRAGON
NS_USING_BOOST

IMPLEMENT_CONTROLLER(Controller, collectionController)

BEGIN_ACTION_MAP(collectionController)
	ACTION(collectionController, upload)
	ACTION(collectionController, test)
END_ACTION_MAP()

void collectionController::test(QueryString &qs)
{
	NamedSemiSpace space(SHARED_MEM_OBJ_NAME, SHARED_MEM_OBJ_SIZE);
	space.Open();
	bool isHeadUsed = space.IsHeadUsed();

	response->StringResult("test " + boost::lexical_cast<std::string>(isHeadUsed));
	space.Close();
}

void collectionController::upload(QueryString &qs)
{
	static bool init = false;
	static Aws::Client::ClientConfiguration config;
	if (init == false) {
		Aws::SDKOptions options;
    		Aws::InitAPI(options);

		config.scheme = Aws::Http::Scheme::HTTPS;
		config.region = Aws::Region::AP_NORTHEAST_1;

		init = true;
	}
	static Aws::SQS::SQSClient sqsClient{config};
	static std::string queueUrl{"https://sqs.ap-northeast-1.amazonaws.com/631794131827/test"};

	if (request->GetMethod() == kHttp_Method_Post && request->GetPost().data != NULL && request->GetPost().len > 0) {
		Log &log = app->GetErrorLog();
		unsigned char *data = request->GetPost().data;

		rapidjson::Document d;
		d.Parse((const char *)data);

		std::string json{"{\"1\" : 1}"};

		Aws::SQS::Model::SendMessageRequest sendMessageRequest1;
		sendMessageRequest1.SetQueueUrl(queueUrl.c_str());
		sendMessageRequest1.SetMessageBody(json.c_str());
		Aws::SQS::Model::SendMessageOutcome sendMessageOutcome = sqsClient.SendMessage(sendMessageRequest1);
		if (!sendMessageOutcome.IsSuccess() || sendMessageOutcome.GetResult().GetMessageId().length() == 0) {
			std::stringstream ss;
			ss << "Erron on send:" << sendMessageOutcome.GetError().GetMessage();
			log.LogMsg(ss.str());
		}
		response->StringResult("Post");
		return ;
	}
	
	response->StringResult("Get");
}