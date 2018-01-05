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
   static String url =  "";
   static String serializedMessage = "";
   static String httpRequest =  "";
   static String httpResponse = "";

	url = message->getString("url");

	if (url != "")
	{
	   message->clearParameter("url");

	   serializedMessage = sendProtocol.serialize(message);

	   httpRequest = "http://" + url + serializedMessage;

	   // Setup the request.
	   http.begin(httpRequest);

	   // Send the request.
	   int httpCode = http.GET();

	   if (httpCode > 0)
	   {
	      // TODO: Parse response.
	      //httpResponse = http.getString(); // TODO: Can fragment heap!

	      Logger::logDebug("HttpClientAdapter::sendRemoteMessage: Response code [%d].", httpCode);
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
