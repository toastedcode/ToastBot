#pragma once

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
};
