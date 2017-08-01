#include "../Messaging/JsonProtocol.hpp"
#include "../Messaging/RestfulProtocol.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "WebServerAdapter.hpp"

WebServerAdapter::WebServerAdapter(
   const String& id,
   const int& port) :
     Adapter(id, 0),
     port(port),
     server(0)
{
   // Nothing to do here.
}

void WebServerAdapter::setup()
{
   Adapter::setup();

   server = new ESP8266WebServer(port);
   server->addHandler(this);
   server->begin();
}

void WebServerAdapter::loop()
{
   Adapter::loop();

   server->handleClient();
}

bool WebServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   static JsonProtocol jsonProtocol;

   server->send(200, "application/json", jsonProtocol.serialize(message));
}

MessagePtr WebServerAdapter::getRemoteMessage()
{
   // Nothing to do here.
   return (0);
}

bool WebServerAdapter::canHandle(
   HTTPMethod method,
   String uri)
{
   Logger::logDebug("WebServerAdapter::canHandle: %s", uri.c_str());
   return (true);
}

bool WebServerAdapter::handle(
   ESP8266WebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   bool success = true;

   static RestfulProtocol restfulProtocol;

   Logger::logDebug("WebServerAdapter::handle: %s", requestUri.c_str());

   MessagePtr message = Messaging::newMessage();
   if ((message) && (restfulProtocol.parse(server, message)))
   {
      message->setSource(getId());
      Messaging::send(message);
   }

   server.send(200);

   return (success);
}
