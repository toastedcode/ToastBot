#pragma once

#include "Adapter.hpp"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"

class UdpAdapter : public Adapter
{

public:

   UdpAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   int port;

   WiFiUDP server;
};

inline UdpAdapter::UdpAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) :
      Adapter(id, protocol),
      port(port)
{
}
