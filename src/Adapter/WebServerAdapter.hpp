#pragma once

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWebServer.hpp"
#include "EspWifi.hpp"
#include "List.hpp"
#include "Webpage.hpp"

class WebServerAdapter : public Adapter, RequestHandler
{

public:

   WebServerAdapter(
      const String& id,
      const int& port);

   WebServerAdapter(
      MessagePtr parameters);

   virtual ~WebServerAdapter();

   // **************************************************************************
   // Component operations

   virtual void setup();

   virtual void loop();

   // **************************************************************************
   // Adapter operations

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   // **************************************************************************
   // RequestHandler operations

   virtual bool canHandle(
      HTTPMethod method,
      String uri);

   virtual bool handle(
      EspWebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   // **************************************************************************

   void addPage(
      Webpage* webpage);

   Webpage* getPage(
      const String& uri);

   void removePage(
      const String& uri);

protected:

   bool servePage(
      EspWebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   bool serveFile(
      EspWebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   bool serveRestfulRequest(
      EspWebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   virtual void handleNotFound(
      EspWebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   static String getDataType(
      const String& path);

private:

   struct PendingRequest
   {
      HTTPMethod requestMethod;

      String requestUri;
   };

   static const int MAX_WEBPAGES = 10;

   static const long RESPONSE_DELAY = 250;  // milliseconds

   int port;

   EspWebServer* server;

   List<Webpage*> webpages;

   PendingRequest pendingRequest;

   long responseTime;
};

REGISTER(WebServerAdapter, WebServerAdapter)
