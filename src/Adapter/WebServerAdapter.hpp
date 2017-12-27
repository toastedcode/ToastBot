#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Webpage.hpp"
#include "Adapter.hpp"
#include "LinkedList.hpp"

class WebServerAdapter : public Adapter, RequestHandler
{

public:

   WebServerAdapter(
      const String& id,
      const int& port);

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
      ESP8266WebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   // **************************************************************************

   bool addPage(
      Webpage* webpage);

   Webpage* getPage(
      const String& uri);

   void removePage(
      const String& uri);

protected:

   bool servePage(
      ESP8266WebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   bool serveFile(
      ESP8266WebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   bool serveRestfulRequest(
      ESP8266WebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

   virtual void handleNotFound(
      ESP8266WebServer& server,
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

   ESP8266WebServer* server;

   LinkedList<Webpage*> webpages;

   PendingRequest pendingRequest;

   long responseTime;
};
