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

   int port;

   WiFiServer server;

   WiFiClient client;
};

inline TcpServerAdapter::TcpServerAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) : Adapter(id, protocol),
                      port(port),
                      server(port)
{
}
