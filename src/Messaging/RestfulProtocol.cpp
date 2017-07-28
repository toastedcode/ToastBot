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

String RestfulProtocol::serialize(
   MessagePtr message) const
{
   String serializedMessage = "";

   // /component/action?param=value;param=value ...

   // component
   if (message->isSet("destination"))
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

   Parameter parameters[MAX_PARAMETERS];
   int paramCount = 0;
   message->getParameters(parameters, paramCount);

   for (int i = 0, paramIndex = 0; i < paramCount; i++)
   {
      Parameter& parameter = parameters[i];

      if ((strncmp(parameter.getName(), "messageId", sizeof(Parameter::ParameterName)) != 0) &&
          (strncmp(parameter.getName(), "destination", sizeof(Parameter::ParameterName)) != 0))
      {
         if (paramIndex == 0)
         {
            serializedMessage += "?";
         }
         else
         {
            serializedMessage += ";";
         }

         serializedMessage += String(parameter.getName());
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
         Parameter parameter(paramName.c_str());

         String lowerCase = valueString;
         lowerCase.toLowerCase();

         if (StringUtils::findFirstNotOf(valueString, "0123456789.-") == -1)
         {
            // TODO: Parse numeric types.
            // TODO: Fix trucation of longs.
            long longValue = valueString.toInt();
            parameter.setValue((int)longValue);
         }
         else if ((lowerCase == "true") ||
                  (lowerCase == "false"))
         {
            parameter.setValue(StringUtils::toBool(valueString));
         }
         else
         {
            parameter.setValue(valueString.c_str());
         }

         parameters[i] = parameter;
         parameterCount++;

         // Next token.
         i++;
         keyValueString = StringUtils::tokenize(paramString, "&");
      }
   }
}
