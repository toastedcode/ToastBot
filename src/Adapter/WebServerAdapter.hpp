#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "Adapter.hpp"

class WebServerAdapter : public Adapter, RequestHandler
{

public:

   WebServerAdapter(
      const String& id,
      const int& port);

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   virtual bool canHandle(
      HTTPMethod method,
      String uri);

   virtual bool handle(
      ESP8266WebServer& server,
      HTTPMethod requestMethod,
      String requestUri);

private:

   int port;

   ESP8266WebServer* server;
};
