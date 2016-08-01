#pragma once

#include "Protocol.hpp"
#include "Set.hpp"

class JsonProtocol : public Protocol
{

public:

   static const int MAX_PARAMETERS = 10;

   bool parse(
      const String& messageString,
      MessagePtr message);

   String serialize(
      MessagePtr message) const;

private:

   typedef Set<Parameter, MAX_PARAMETERS> ParameterSet;

   String serializeParameters(
      MessagePtr message) const;

   static bool parseParameters(
      const String& parameterString,
      ParameterSet& parameters);

   static bool parseParameter(
      const String& parameterString,
      Parameter& parameter);

   static ParameterType getType(
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
