#pragma once

#include <ESP8266WebServer.h>

#include "Protocol.hpp"

class RestfulProtocol : public Protocol
{

public:

   RestfulProtocol();

   virtual ~RestfulProtocol();

   bool parse(
      const String& messageString,
      MessagePtr message);

   bool parse(
      ESP8266WebServer& server,
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
