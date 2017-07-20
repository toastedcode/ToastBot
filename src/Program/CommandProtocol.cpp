#include "CommandProtocol.hpp"
#include "StringUtils.hpp"

CommandProtocol::CommandProtocol()
{

}

CommandProtocol::~CommandProtocol()
{

}

bool CommandProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   // componentId.action(parameter, parameter, parameter)

   bool success = false;

   String parsedString = messageString;

   int parameterCount = 0;
   String parameters[MAX_PARAMETERS];

   String componentId = parseComponent(messageString);
   String action = parseAction(messageString);
   parseParameters(messageString, parameters, parameterCount);

   if (action != "")
   {
      // Syntax error.
   }
   else
   {
      message->setMessageId("instruction");
      message->setDestination(componentId);
      message->set("action", action);

      // Parameters.
      int paramIndex = 0;
      for (int index; index < parameterCount; index++)
      {
         String name = "param_" + String(index);
         message->set(name, parameters[index]);
      }

      success = true;
   }

   return (success);
}

String CommandProtocol::serialize(
   MessagePtr message) const
{
   String messageString = message->getDestination() + "." + message->getMessageId();

   // Parameters.
   messageString += "(";
   int index = 0;
   String name = "parameter" + String(index);
   while (message->isSet(name))
   {
      messageString += message->getString(name);
      messageString += ", ";  // TODO
   }
   messageString += ");";

   return (messageString);
}

String CommandProtocol::parseComponent(
   const String& messageString)
{
   return ("");
}

String CommandProtocol::parseAction(
   const String& messageString)
{
   return ("");
}

void CommandProtocol::parseParameters(
   const String& messageString,
   String parameters[MAX_PARAMETERS],
   int& parameterCount)
{
   parameterCount = 0;

   int startPos = StringUtils::findFirstOf(messageString, "(");
   int endPos = StringUtils::findFirstOf(messageString, ")");
   String paramString = messageString.substring(startPos, endPos);

   if (paramString != "")
   {
      String param = StringUtils::tokenize(paramString, ",");
      while (param != "")
      {
         parameters[parameterCount] = StringUtils::removeAll(param, " ");
         parameterCount++;
      }
   }
}
