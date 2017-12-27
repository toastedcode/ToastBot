#include "JsonProtocol.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"

const int JsonProtocol::MAX_PARAMETERS;

bool JsonProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   bool isSuccess = false;

   String remainingString = messageString;
   Parameter parameters[MAX_PARAMETERS];
   int parameterCount = 0;

   Logger::logDebugFinest(F("JsonProtocol::parse: Parsing: \"%s\"."), messageString.c_str());

   // Strip braces and whitespace.
   remainingString = StringUtils::removeAll(remainingString, " \n\r\t{}");

   // Parse parameters.
   if (parseParameters(remainingString, parameters, parameterCount) == false)
   {
      Logger::logWarning(F("JsonProtocol::parse: Failed to parse parameters."));
   }
   else
   {
      // Populate message with parsed parameters.
      for (int i = 0; i < parameterCount; i++)
      {
         message->setParameter(parameters[i]);
      }

      isSuccess = true;
   }

   return (isSuccess);
}

String JsonProtocol::serialize(
   MessagePtr message) const
{
   String serializedMessage = "";

   serializedMessage += "{";

   serializedMessage += serializeParameters(message);

   serializedMessage += "}";

   Logger::logDebugFinest(F("JsonProtocol::serialize: Serialized: \"%s\"."), serializedMessage.c_str());

   return (serializedMessage);
}

String JsonProtocol::serializeParameters(
   MessagePtr message) const
{
   String serializedParameters = "";

   const List<Parameter>& parameters = message->getParameters();
   int paramIndex = 0;

   for (List<Parameter>::Iterator it = parameters.begin(); it != parameters.end(); it++, paramIndex++)
   {
      String parameterValue = "";

      switch (it->getType())
      {
         case Parameter::BOOL:
         {
            parameterValue = String(it->getBoolValue());
            break;
         }

         case Parameter::DOUBLE:
         {
            parameterValue = String(it->getDoubleValue());
            break;
         }

         case Parameter::FLOAT:
         {
            parameterValue = String(it->getFloatValue());
            break;
         }

         case Parameter::INT:
         {
            parameterValue = String(it->getIntValue());
            break;
         }

         case Parameter::STRING:
         {
            parameterValue = wrap(it->getStringValue(), '\"');
            break;
         }

         default:
         {
            break;
         }
      }

      serializedParameters += (wrap(it->getName(), '\"') + ":" + parameterValue);

      if (paramIndex < (parameters.length() - 1))
      {
         serializedParameters += ", ";
      }
   }

   return (serializedParameters);
}

bool JsonProtocol::parseParameters(
   const String& parameterString,
   Parameter parameters[],
   int& count)
{
   bool isSuccess = true;

   String remainingString = parameterString;
   Parameter parameter;

   while (isSuccess && (remainingString != ""))
   {
      isSuccess = parseParameter(StringUtils::tokenize(remainingString, ","), parameter);

      if (isSuccess)
      {
         parameters[count] = parameter;
         count++;
      }
   }

   return (isSuccess);
}

bool JsonProtocol::parseParameter(
   const String& parameterString,
   Parameter& parameter)
{
   bool isSuccess = false;

   String remainingString = parameterString;

   String name = StringUtils::tokenize(remainingString, ":");
   String value = remainingString;

   if (validName(name) && validValue(value))
   {
      parameter.setName(stripQuotes(name).c_str());

      switch (getType(value))
      {
         case Parameter::BOOL:
         {
            parameter.setValue(StringUtils::toBool(value));
            break;
         }

         case Parameter::DOUBLE:
         {
            // TODO: Handle double
            parameter.setValue(value.toFloat());
            break;
         }

         case Parameter::FLOAT:
         {
            parameter.setValue(value.toFloat());
            break;
         }

         case Parameter::INT:
         {
            // TODO: Fix trucation of longs.
            long longValue = value.toInt();
            parameter.setValue((int)longValue);
            break;
         }

         case Parameter::STRING:
         {
            parameter.setValue(stripQuotes(value).c_str());
            break;
         }

         default:
         {
            break;
         }
      }

      isSuccess = true;
   }
   else
   {
      Logger::logWarning(F("JsonProtocol::parse: Failed to parse parameter: \"%s\"."), parameterString.c_str());
   }

   return (isSuccess);
}

Parameter::ParameterType JsonProtocol::getType(
   const String& value)
{
   Parameter::ParameterType type = Parameter::STRING;

   String lowerCase = value;
   lowerCase.toLowerCase();

   if ((value.charAt(0) == '\"') &&
       (value.charAt(value.length() - 1) == '\"'))
   {
      type = Parameter::STRING;
   }
   else if (StringUtils::findFirstNotOf(value, "0123456789.-") == -1)
   {
      // TODO: Parse numeric types.
      type = Parameter::INT;
   }
   else if ((lowerCase == "true") ||
            (lowerCase == "false"))
   {
      type = Parameter::BOOL;
   }
   else
   {
      Logger::logDebugFinest(F("JsonProtocol::getType: Treating unquoted value [%s] as STRING."), value.c_str());
   }

   return (type);
}

bool JsonProtocol::validName(
   const String& name)
{
   // TODO: More
   return (name != "");
}

bool JsonProtocol::validValue(
   const String& value)
{
   // TODO: More
   return (value != "");
}

inline String JsonProtocol::stripQuotes(
   const String& value)
{
   return (StringUtils::removeAll(value, "\"\'"));
}
