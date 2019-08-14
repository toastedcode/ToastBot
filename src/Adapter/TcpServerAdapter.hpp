#pragma once

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWifi.hpp"

class TcpServerAdapter : public Adapter
{

public:

   TcpServerAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   TcpServerAdapter(
      MessagePtr parameters);

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

REGISTER(TcpServerAdapter, TcpServerAdapter)
