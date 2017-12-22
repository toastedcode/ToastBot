#include "Logger.hpp"
#include "Messaging.hpp"
#include "HttpClientAdapter.hpp"

HttpClientAdapter::HttpClientAdapter(
   const String& id,
   Protocol* protocol) :
   Adapter(id, protocol)
{
}

bool HttpClientAdapter::sendRemoteMessage(
   MessagePtr message)
{
	String url = message->getString("url");

	if (url != "")
	{
	   String serializedMessage = sendProtocol.serialize(message);

	   String httpRequest = "http://" + url + serializedMessage;

	   // Setup the request.
	   http.begin("http://" + url + serializedMessage);

	   // Send the request.
	   int httpCode = 0;//http.GET();

	   if (httpCode > 0)
	   {
	      String httpResponse = http.getString();

	      // TODO: Parse response.
	      Logger::logDebug("HttpClientAdapter::sendRemoteMessage: Response [%d] = %s", httpCode, httpResponse.c_str());
	   }
	   else
	   {
	      Logger::logWarning("No response from request: %s.", httpRequest.c_str());
	   }

	   http.end();
	}
	else
	{
	   Logger::logWarning("No URL specified in HTTP request [%s].", message->getMessageId().c_str());
	}

   return (true);
}

MessagePtr HttpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   // TODO: Read message from queued responses.

   return (message);
}
