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
   Logger::logDebug("HttpClientAdapter::HttpClientAdapter: %s", defaultUrl.c_str());
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
        // TODO: Parse response.
        //httpResponse = http.getString(); // TODO: Can fragment heap!

        Logger::logDebug(F("HttpClientAdapter::sendRemoteMessage: Request: %s"), httpRequest.c_str());

        Logger::logDebug(F("HttpClientAdapter::sendRemoteMessage: Response code [%d]."), httpCode);
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
