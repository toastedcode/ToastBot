#pragma once

#include "Adapter.hpp"
#include "ESP8266WiFi.h"

class TcpServerAdapter : public Adapter
{

public:

   TcpServerAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   static const int BUFFER_SIZE = 256;

   int port;

   WiFiServer server;

   WiFiClient client;

   bool isConnected;

   char buffer[BUFFER_SIZE];

   int readIndex = 0;
};
