#include "CommandProtocol.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"

// *****************************************************************************
//                                Public
// *****************************************************************************

const String PARAM_PREFIX = "param_";

CommandProtocol::CommandProtocol()
{
   // Nothing to do here.
}

CommandProtocol::~CommandProtocol()
{
   // Nothing to do here.
}

bool CommandProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   bool success = false;

   // component.action(param, param, ...);

   String componentId = parseComponent(messageString);
   if (componentId.length() > 0)
   {
      message->setDestination(componentId);
   }

   String action = parseAction(messageString);
   if (action.length() > 0)
   {
      message->setMessageId(FPSTR(INSTRUCTION));
      message->set("action", action);
      success = true;  // TODO: More validation.
   }

   parseParameters(messageString, message);

   return (success);
}

String CommandProtocol::serialize(
   MessagePtr message) const
{
   String serializedMessage = "";

   // component.action(param, param, ...);

   // component
   if (message->isSet(FPSTR(DESTINATION)))
   {
      serializedMessage += message->getDestination();
      serializedMessage += ".";
   }

   // action
   serializedMessage += message->getMessageId();

   //
   // parameters
   //

   serializedMessage += "(";
   int i = 0;
   String paramName = PARAM_PREFIX + String(i);
   while (message->isSet(paramName))
   {
      if (i > 0)
      {
         serializedMessage += ", ";
      }

      serializedMessage += message->getString(paramName);

      i++;
      paramName = PARAM_PREFIX + String(i);
   }

   serializedMessage += ");";

   return (serializedMessage);
}

// *****************************************************************************
//                                Private
// *****************************************************************************

String CommandProtocol::parseComponent(
   const String& messageString)
{
   String componentId = "";

   int endPos = StringUtils::findFirstOf(messageString, "(");
   if ((endPos != -1) &&
       (endPos != 0))
   {
      String commandString = messageString.substring(0, endPos - 1);

      String token1 = StringUtils::tokenize(commandString, ".");
      String token2 = StringUtils::tokenize(commandString, ".");

      if (token2.length() != 0)
      {
         componentId = token1;
      }
   }

   return (componentId);
}

String CommandProtocol::parseAction(
   const String& messageString)
{
   String action = "";

   int endPos = StringUtils::findFirstOf(messageString, "(");
   if ((endPos != -1) &&
       (endPos != 0))
   {
      String commandString = messageString.substring(0, endPos);

      String token1 = StringUtils::tokenize(commandString, ".");
      String token2 = StringUtils::tokenize(commandString, ".");

      if (token2.length() != 0)
      {
         action = token2;
      }
      else
      {
         action = token1;
      }
   }

   return (action);
}

void CommandProtocol::parseParameters(
   const String& messageString,
   MessagePtr message)
{
   int startPos = StringUtils::findFirstOf(messageString, "(") + 1;
   int endPos = StringUtils::findFirstOf(messageString, ")");
   if ((startPos != -1) &&
       (endPos != -1) &&
       ((endPos - startPos) > 0))
   {
      String paramString = messageString.substring(startPos, endPos);

      int i = 0;
      String paramValue = StringUtils::tokenize(paramString, ",");

      while (paramValue.length() > 0)
      {
         String paramName = PARAM_PREFIX + String(i);
         paramValue.trim();

         message->set(paramName, paramValue);

         // Next token.
         i++;
         paramValue = StringUtils::tokenize(paramString, ",");
      }
   }
}
