#pragma once

#include "Protocol.hpp"

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
      List<Parameter>&  parameters);

   static bool parseParameter(
      const String& parameterString,
      Parameter& parameter);

   static Parameter::ParameterType getType(
      const String& value);

   static bool validName(
      const String& name);

   static bool validValue(
      const String& value);

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
