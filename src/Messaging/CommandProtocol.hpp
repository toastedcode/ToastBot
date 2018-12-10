#pragma once

#include "Protocol.hpp"
#include "ProtocolFactory.hpp"

class CommandProtocol : public Protocol
{

public:

   CommandProtocol();

   virtual ~CommandProtocol();

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
      MessagePtr message);
};

REGISTER_PROTOCOL(CommandProtocol, CommandProtocol)
