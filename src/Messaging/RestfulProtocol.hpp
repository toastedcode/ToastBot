#pragma once

#include "EspWebServer.hpp"
#include "Protocol.hpp"
#include "ProtocolFactory.hpp"

class RestfulProtocol : public Protocol
{

public:

   RestfulProtocol();

   virtual ~RestfulProtocol();

   bool parse(
      const String& messageString,
      MessagePtr message);

   bool parse(
      EspWebServer& server,
      MessagePtr message);

   String serialize(
      MessagePtr message) const;

private:

   static String parseComponent(
      const String& messageString);

   static String parseAction(
      const String& messageString);

   static void parseParameters(
      const String& messageString,
      MessagePtr message);
};

REGISTER_PROTOCOL(RestfulProtocol, RestfulProtocol)
