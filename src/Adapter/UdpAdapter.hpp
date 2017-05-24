#pragma once

#include "Adapter.hpp"
#include "ESP8266WiFi.h"
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

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   int listenPort;

   IPAddress sendIpAddress;

   int sendPort;

   WiFiUDP server;

private:

   // This operation creates a "reply adapter id" which
   // can be used to reply to a message received by this adapter.
   String getReplyAdapterId(
      // The IP address to use when sending a reply.
      const IPAddress& ipAddress,
      // The port to use when sending a reply.
      const int& port) const;

   void parseReplyAdapterId(
      const String& replyAdapterId,
      IPAddress& ipAddress,
      int& port);
};
