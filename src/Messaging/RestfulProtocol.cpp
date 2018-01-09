#include "Logger.hpp"
#include "RestfulProtocol.hpp"
#include "StringUtils.hpp"

// *****************************************************************************
//                                Public
// *****************************************************************************

RestfulProtocol::RestfulProtocol()
{
   // Nothing to do here.
}

RestfulProtocol::~RestfulProtocol()
{
   // Nothing to do here.
}

bool RestfulProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   bool success = false;

   // /component/action?param=value&param=value ...

   String componentId = parseComponent(messageString);
   if (componentId.length() > 0)
   {
      message->setDestination(componentId);
   }

   String action = parseAction(messageString);
   if (action.length() > 0)
   {
      message->setMessageId(action);
      success = true;  // TODO: More validation.
   }

   parseParameters(messageString, message);

   return (success);
}

bool RestfulProtocol::parse(
   ESP8266WebServer& server,
   MessagePtr message)
{
   bool success = false;

   // /component/action?param=value&param=value ...

   // component
   String componentId = parseComponent(server.uri());
   if (componentId.length() > 0)
   {
      message->setDestination(componentId);
   }

   // action
   String action = parseAction(server.uri());
   if (action.length() > 0)
   {
      message->setMessageId(action);
      success = true;  // TODO: More validation.
   }

   // paramters
   int argCount = server.args();
   for (int i = 0; i < argCount; i++)
   {
      message->set(server.argName(i), server.arg(i));
   }

   return (success);
}

String RestfulProtocol::serialize(
   MessagePtr message) const
{
   String serializedMessage = "";

   // /component/action?param=value;param=value ...

   // component
   if (message->getDestination() != "")
   {
      serializedMessage += "/";
      serializedMessage += message->getDestination();
   }

   // action
   serializedMessage += "/";
   serializedMessage += message->getMessageId();

   //
   // parameters
   //

   int paramIndex = 0;

   for (Message::Iterator it = message->begin(); it != message->end(); it++)
   {
      if ((it->first != "messageId") &&
          (it->first != "destination"))
      {
         if (paramIndex == 0)
         {
            serializedMessage += "?";
         }
         else
         {
            serializedMessage += "&";
         }

         serializedMessage += it->first;
         serializedMessage += "=";
         serializedMessage += it->second;

         paramIndex++;
      }
   }

   return (serializedMessage);
}

// *****************************************************************************
//                                Private
// *****************************************************************************

String RestfulProtocol::parseComponent(
   const String& messageString)
{
   String componentId = "";

   int startPos = StringUtils::findFirstOf(messageString, "/");

   if (startPos != -1)
   {
      int endPos = StringUtils::findFirstOf(messageString, "?", startPos);
      endPos = (endPos == -1) ? messageString.length() : endPos;

      String commandString = messageString.substring(0, endPos - 1);

      String token1 = StringUtils::tokenize(commandString, "/");
      String token2 = StringUtils::tokenize(commandString, "/");

      if (token2.length() != 0)
      {
         componentId = token1;
      }
   }

   return (componentId);
}

String RestfulProtocol::parseAction(
   const String& messageString)
{
   String action = "";

   int startPos = StringUtils::findFirstOf(messageString, "/");

   if (startPos != -1)
   {
      int endPos = StringUtils::findFirstOf(messageString, "?", startPos);
      endPos = (endPos == -1) ? messageString.length() : endPos;

      String commandString = messageString.substring(0, endPos);

      String token1 = StringUtils::tokenize(commandString, "/");
      String token2 = StringUtils::tokenize(commandString, "/");

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

void RestfulProtocol::parseParameters(
   const String& messageString,
   MessagePtr message)
{
   int startPos = StringUtils::findFirstOf(messageString, "?");

   if ((startPos != -1) &&
       (startPos < messageString.length()))
   {
      String paramString = messageString.substring(startPos + 1);

      String keyValueString = StringUtils::tokenize(paramString, "&");

      while (keyValueString.length() > 0)
      {
         String paramName = StringUtils::tokenize(keyValueString, "=");
         String paramValue = StringUtils::tokenize(keyValueString, "=");

         message->set(paramName, paramValue);

         // Next token.
         keyValueString = StringUtils::tokenize(paramString, "&");
      }
   }
}
