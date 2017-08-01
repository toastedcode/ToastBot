#pragma once

#include <ESP8266WebServer.h>

#include "Protocol.hpp"

class RestfulProtocol : public Protocol
{

public:

   static const int MAX_PARAMETERS = 10;

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
      Parameter parameters[MAX_PARAMETERS],
      int& parameterCount);

   static Parameter parseParameter(
      const String& name,
      const String& value);
};
