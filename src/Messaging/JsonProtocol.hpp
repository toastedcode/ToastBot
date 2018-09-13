#pragma once

#include "Protocol.hpp"
#include "ProtocolFactory.hpp"

class JsonProtocol : public Protocol
{

public:

   bool parse(
      const String& messageString,
      MessagePtr message);

   String serialize(
      MessagePtr message) const;

private:

   String serializeParameters(
      MessagePtr message) const;

   static bool parseParameters(
      const String& parameterString,
      MessagePtr message);

   static bool parseParameter(
      const String& parameterString,
      MessagePtr message);

   static String stripQuotes(
      const String& value);

   static void stripWhitespace(
      const String& value);

   static String wrap(
      const String& value,
      const char& c);
};

inline String JsonProtocol::wrap(
   const String& value,
   const char& c)
{
   return (String(c) + value + String(c));
}

REGISTER_PROTOCOL(JsonProtocol, JsonProtocol)
