#pragma once

#include "Protocol.hpp"
#include "Set.hpp"

class RestfulProtocol : public Protocol
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
};
