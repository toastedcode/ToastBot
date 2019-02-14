#include "HttpClientAdapter.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"

HttpClientAdapter::HttpClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& defaultUrl) :
      Adapter(id, protocol),
      defaultUrl(defaultUrl)
{
}

HttpClientAdapter::HttpClientAdapter(
   MessagePtr parameters) :
      Adapter(parameters)
{
   defaultUrl = parameters->getString("defaultUrl");
}

bool HttpClientAdapter::sendRemoteMessage(
   MessagePtr message)
{
   static String url =  "";
   static String serializedMessage = "";
   static String httpRequest =  "";
   static String httpResponse = "";

   url = message->getString("url");
   if (url.length() == 0)
   {
      url = defaultUrl;
   }

   if (url != "")
   {
     message->erase("url");

     serializedMessage = sendProtocol.serialize(message);

     httpRequest = "http://" + url + serializedMessage;

     // Setup the request.
     http.begin(httpRequest);

     // Send the request.
     int httpCode = http.GET();

     if (httpCode > 0)
     {
        Logger::logDebugFinest(F("HttpClientAdapter::sendRemoteMessage: Response code [%d]."), httpCode);

        httpResponse = http.getString();
        Logger::logDebugFinest(F("HttpClientAdapter::sendRemoteMessage: Response: %s"), httpResponse.c_str());
     }
     else
     {
        Logger::logWarning(F("No response from request: %s"), httpRequest.c_str());
     }

     http.end();
   }
   else
   {
     Logger::logWarning(F("No URL specified in HTTP request [%s]."), message->getMessageId().c_str());
   }

   return (true);
}

MessagePtr HttpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   // TODO: Read message from queued responses.

   return (message);
}
