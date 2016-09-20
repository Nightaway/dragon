#include "collectionController.h"

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <aws/sqs/model/GetQueueUrlRequest.h>
#include <aws/sqs/model/CreateQueueRequest.h>

#include "../config.h"
#include "../Utility/rapidjson/document.h"
#include "../Utility/rapidjson/writer.h"
#include "../Utility/rapidjson/stringbuffer.h"

NS_USING_DRAGON

IMPLEMENT_CONTROLLER(Controller, collectionController)

BEGIN_ACTION_MAP(collectionController)
	ACTION(collectionController, upload)
END_ACTION_MAP()

static std::string returnJson(std::string msg)
{
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Value k_status("status");
	rapidjson::Value v_status;
	v_status.SetString(msg.c_str(), msg.length(), d.GetAllocator());
	d.AddMember(k_status, v_status, d.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	return buffer.GetString();
}

void collectionController::upload(QueryString &qs)
{
	static bool init = false;
	static Aws::Client::ClientConfiguration config;
	if (init == false) {
		Aws::SDKOptions options;
    		Aws::InitAPI(options);

		config.scheme = Aws::Http::Scheme::HTTPS;
		config.region = Aws::Region::US_EAST_1;
		config.connectTimeoutMs = 60000;
		config.requestTimeoutMs = 60000;

		init = true;
	}
	static Aws::SQS::SQSClient sqsClient(Aws::Auth::AWSCredentials("AKIAJXLUWGXZ34TW7REQ", "ELV6V/bg+bg9rOOZYb47HN2oNqac3zGFSgDDXTTC"), config);

	if (request->GetMethod() == kHttp_Method_Post && request->GetPost().data != NULL && request->GetPost().len > 0) {
		Log &log = app->GetErrorLog();
		unsigned char *data = request->GetPost().data;

		rapidjson::Document d;
		d.Parse((const char *)data);

		if (d.IsObject()) {
			if (d.HasMember("datas") && d["datas"].IsArray()) {
				for (int i=0; i<d["datas"].Size(); i++) {
					const rapidjson::Value &e = d["datas"][i];
					if (e.HasMember("type") && e["type"].IsString()) {
						Aws::String queueUrl;
						std::string type = e["type"].GetString();
						std::string dc = "dc-";
						std::string queueName = dc + type;
						
						Aws::SQS::Model::GetQueueUrlRequest getUrlRequest;
						getUrlRequest.SetQueueName(queueName.c_str());
						Aws::SQS::Model::GetQueueUrlOutcome getUrlOutcome = sqsClient.GetQueueUrl(getUrlRequest);
						if (!getUrlOutcome.IsSuccess()) {
							std::stringstream ss;
							ss << "Erron on getUrl:" << getUrlOutcome.GetError().GetMessage();
							log.LogMsg(ss.str());

							Aws::SQS::Model::CreateQueueRequest createRequest;
							createRequest.SetQueueName(queueName.c_str());
							Aws::SQS::Model::CreateQueueOutcome createQueueOutcome = sqsClient.CreateQueue(createRequest);
							if (!createQueueOutcome.IsSuccess()) {
								std::stringstream ss;
								ss << "Erron on createQueue:" << createQueueOutcome.GetError().GetMessage();
								log.LogMsg(ss.str());
								return ;
							} else {
								Aws::SQS::Model::CreateQueueResult result = createQueueOutcome.GetResult();
								queueUrl = result.GetQueueUrl();
							}
						} else {
							Aws::SQS::Model::GetQueueUrlResult result = getUrlOutcome.GetResult();
							queueUrl = result.GetQueueUrl();
						}

						rapidjson::Document res;
						const rapidjson::Value &event = e["event"];
						res.SetObject();
						res.AddMember("key", d["key"], res.GetAllocator());
						res.AddMember("common", d["common"], res.GetAllocator());
						rapidjson::Value k_event("event");
						rapidjson::Value v_event;
						v_event.SetString(event.GetString(), event.GetStringLength(), res.GetAllocator());
						res.AddMember(k_event, v_event, res.GetAllocator());
						rapidjson::Value v_timestamp;
						v_timestamp.SetInt(time(NULL));
						res.AddMember("timestamp", v_timestamp, res.GetAllocator());

						rapidjson::StringBuffer buffer;
						rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
						res.Accept(writer);

						Aws::SQS::Model::SendMessageRequest sendMessageRequest1;
						sendMessageRequest1.SetQueueUrl(queueUrl);
						sendMessageRequest1.SetMessageBody(buffer.GetString());
						Aws::SQS::Model::SendMessageOutcome sendMessageOutcome = sqsClient.SendMessage(sendMessageRequest1);
						if (!sendMessageOutcome.IsSuccess() || sendMessageOutcome.GetResult().GetMessageId().length() == 0) {
							std::stringstream ss;
							ss << "Erron on send:" << sendMessageOutcome.GetError().GetMessage();
							log.LogMsg(ss.str());
						}
					}
				}
			}
		}

		response->StringResult(returnJson("OK"));
		return ;
	}
	
	response->StringResult(returnJson("GET"));
}