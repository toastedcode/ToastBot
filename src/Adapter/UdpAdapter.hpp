#pragma once

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWifi.hpp"
#include "WiFiUdp.h"

class UdpAdapter : public Adapter
{

public:

   // Constructor for UDP listen-only adapter.
   UdpAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   // Constructor for UDP send-only adapter.
   UdpAdapter(
      const String& id,
      Protocol* protocol,
      const IPAddress& ipAddress,
      const int& port);

   UdpAdapter(
      MessagePtr parameters);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   int listenPort;

   IPAddress sendIpAddress;

   int sendPort;

   WiFiUDP server;

private:

   // This operation returns a stringified version of an IP address and port.
   // Format xxx.xxx.xxx.xxx:xx
   static String getSocketString(
      // The IP address to use when sending a reply.
      const IPAddress& ipAddress,
      // The port to use when sending a reply.
      const int& port);

   // This operation parses a string into an IP address and port.
   // Expected format xxx.xxx.xxx.xxx:xx
   static void parseSocketString(
      const String& destination,
      IPAddress& ipAddress,
      int& port);
};

REGISTER(UdpAdapter, UdpAdapter)
