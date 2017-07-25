#pragma once

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "ESP8266WiFi.h"

class TcpClientAdapter : public Adapter
{

public:

   TcpClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& host,
      const int& port,
      const int& retryDelay);

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   bool connect();

   bool disconnect();

private:

   static const int BUFFER_SIZE = 256;

   String host;

   int port;

   WiFiClient client;

   long retryTime;

   int retryDelay;  // milliseconds

   char buffer[BUFFER_SIZE];

   int readIndex = 0;
};
