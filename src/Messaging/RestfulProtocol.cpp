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

   // /component/action?param=value;param=value ...

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

   Parameter parameters[MAX_PARAMETERS];
   int parameterCount = 0;
   parseParameters(messageString, parameters, parameterCount);

   for (int i = 0; i < parameterCount; i++)
   {
      message->setParameter(parameters[i]);
   }

   return (success);
}

bool RestfulProtocol::parse(
   ESP8266WebServer& server,
   MessagePtr message)
{
   bool success = false;

   // /component/action?param=value;param=value ...

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
      String name = server.argName(i);
      String value = server.arg(i);

      message->setParameter(parseParameter(name, value));
   }

   return (success);
}

String RestfulProtocol::serialize(
   MessagePtr message) const
{
   String serializedMessage = "";

   // /component/action?param=value;param=value ...

   // component
   if (message->isSet("destination") && (message->getDestination() != ""))
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

   const List<Parameter>& parameters = message->getParameters();
   int paramIndex = 0;

   for (List<Parameter>::Iterator it = parameters.begin(); it != parameters.end(); it++, paramIndex++)
   {
      Parameter& parameter = *it;

      if ((parameter.getName() != "messageId") &&
          (parameter.getName() != "destination"))
      {
         if (paramIndex == 0)
         {
            serializedMessage += "?";
         }
         else
         {
            serializedMessage += "&";
         }

         serializedMessage += parameter.getName();
         serializedMessage += "=";

         switch (parameter.getType())
         {
            case Parameter::BOOL:
            {
               serializedMessage += String(parameter.getBoolValue());
               break;
            }

            case Parameter::DOUBLE:
            {
               serializedMessage += String(parameter.getDoubleValue());
               break;
            }

            case Parameter::FLOAT:
            {
               serializedMessage += String(parameter.getFloatValue());
               break;
            }

            case Parameter::INT:
            {
               serializedMessage += String(parameter.getIntValue());
               break;
            }

            case Parameter::STRING:
            {
               serializedMessage += parameter.getStringValue();
               break;
            }

            case Parameter::UNKNOWN:
            default:
            {
               // Invalid.
            }
         }

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
   Parameter parameters[MAX_PARAMETERS],
   int& parameterCount)
{
   int startPos = StringUtils::findFirstOf(messageString, "?");

   if ((startPos != -1) &&
       (startPos < messageString.length()))
   {
      String paramString = messageString.substring(startPos + 1);

      int i = 0;
      String keyValueString = StringUtils::tokenize(paramString, ";");

      while ((keyValueString.length() > 0) &&
             (i < MAX_PARAMETERS))
      {
         String paramName = StringUtils::tokenize(keyValueString, "=");
         String valueString = StringUtils::tokenize(keyValueString, "=");

         parameters[i] = parseParameter(paramName, valueString);
         parameterCount++;

         // Next token.
         i++;
         keyValueString = StringUtils::tokenize(paramString, "&");
      }
   }
}

Parameter RestfulProtocol::parseParameter(
   const String& name,
   const String& value)
{
   Parameter parameter;

   String lowerCase = value;
   lowerCase.toLowerCase();

   parameter.setName(name.c_str());

   if (StringUtils::findFirstNotOf(value, "0123456789.-") == -1)
   {
      // TODO: Parse numeric types.
      // TODO: Fix trucation of longs.
      long longValue = value.toInt();
      parameter.setValue((int)longValue);
   }
   else if ((lowerCase == "true") ||
            (lowerCase == "false"))
   {
      parameter.setValue(StringUtils::toBool(value));
   }
   else
   {
      parameter.setValue(value.c_str());
   }

   return (parameter);
}
