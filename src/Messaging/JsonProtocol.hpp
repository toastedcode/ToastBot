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

   struct Parameter
   {
      String name;

      String value;

      bool operator==(const Parameter& rhs) const
      {
         return ((name == rhs.name) && (value == rhs.value));
      }
   };

   typedef Set<Parameter, MAX_PARAMETERS> ParameterSet;

   static bool parseParameters(
      const String& parameterString,
      ParameterSet& parameters);

   static bool parseParameter(
      const String& parameterString,
      Parameter& parameter);

   static bool validName(
      const String& name);

   static bool validValue(
      const String& value);

   static void stripQuotes(
      const String& value);

   static void stripWhitespace(
      const String& value);
};
