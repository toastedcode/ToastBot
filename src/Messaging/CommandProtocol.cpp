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

   Parameter parameters[MAX_PARAMETERS];
   int parameterCount = 0;
   parseParameters(messageString, parameters, parameterCount);

   for (int i = 0; i < parameterCount; i++)
   {
      message->setParameter(parameters[i]);
   }

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
   while ((message->isSet(paramName)) &&
          (i < MAX_PARAMETERS))
   {
      if (i > 0)
      {
         serializedMessage += ", ";
      }

      Parameter parameter = message->getParameter(paramName);

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
   Parameter parameters[MAX_PARAMETERS],
   int& parameterCount)
{
   int startPos = StringUtils::findFirstOf(messageString, "(") + 1;
   int endPos = StringUtils::findFirstOf(messageString, ")");
   if ((startPos != -1) &&
       (endPos != -1) &&
       ((endPos - startPos) > 0))
   {
      String paramString = messageString.substring(startPos, endPos);

      int i = 0;
      String valueString = StringUtils::tokenize(paramString, ",");
      while ((valueString.length() > 0) &&
             (i < MAX_PARAMETERS))
      {
         valueString.trim();

         String lowerCase = valueString;
         lowerCase.toLowerCase();

         String paramName = PARAM_PREFIX + String(i);
         Parameter parameter(paramName.c_str());

         if ((valueString.charAt(0) == '\"') &&
             (valueString.charAt(valueString.length() - 1) == '\"'))
         {
            valueString = valueString.substring(1, (valueString.length() - 1));
            parameter.setValue(valueString.c_str());
         }
         else if (StringUtils::findFirstNotOf(valueString, "0123456789.-") == -1)
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
            Logger::logWarning(F("CommandProtocol::parseParameters: Bad parameter [\"%s\"]."), valueString.c_str());
         }

         parameters[i] = parameter;
         parameterCount++;

         // Next token.
         i++;
         valueString = StringUtils::tokenize(paramString, ",");
      }
   }
}
