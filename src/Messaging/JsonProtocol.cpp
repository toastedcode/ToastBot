#include "JsonProtocol.hpp"
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

#ifdef MESSAGING_DEBUG
   printf("JsonProtocol::parse: Parsing: \"%s\".\n", messageString.c_str());
#endif

   // Strip braces and whitespace.
   remainingString = StringUtils::removeAll(remainingString, " \n\r\t{}");

   // Parse parameters.
   if (parseParameters(remainingString, parameters, parameterCount) == false)
   {
#ifdef MESSAGING_DEBUG
      printf("JsonProtocol::parse: Failed to parse parameters.\n");
#endif
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

#ifdef MESSAGING_DEBUG
   printf("JsonProtocol::serialize: Serialized: \"%s\".\n", serializedMessage.c_str());
#endif

   return (serializedMessage);
}

String JsonProtocol::serializeParameters(
   MessagePtr message) const
{
   String serializedParameters = "";

   Parameter parameters[MAX_PARAMETERS];
   int parameterCount = 0;
   message->getParameters(parameters, parameterCount);

   for (int i = 0; i < parameterCount; i++)
   {
      String parameterValue = "";

      switch (parameters[i].getType())
      {
         case Parameter::BOOL:
         {
            parameterValue = String(parameters[i].getBoolValue());
            break;
         }

         case Parameter::DOUBLE:
         {
            parameterValue = String(parameters[i].getDoubleValue());
            break;
         }

         case Parameter::FLOAT:
         {
            parameterValue = String(parameters[i].getFloatValue());
            break;
         }

         case Parameter::INT:
         {
            parameterValue = String(parameters[i].getIntValue());
            break;
         }

         case Parameter::STRING:
         {
            parameterValue = wrap(parameters[i].getStringValue(), '\"');
            break;
         }

         default:
         {
            break;
         }
      }

      serializedParameters += (wrap(parameters[i].getName(), '\"') + ":" + parameterValue);

      if (i < (parameterCount - 1))
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
            // TODO:
            //parameter.setValue(value.toDouble());
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
#ifdef MESSAGING_DEBUG
      printf("JsonProtocol::parse: Failed to parse parameter: \"%s\".\n", parameterString.c_str());
#endif
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
   else if (StringUtils::findFirstNotOf(value, "123456789.-") == -1)
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
#ifdef MESSAGING_DEBUG
      printf("JsonProtocol::getType: Treating unquoted value [%s] as STRING.\n", value.c_str());
#endif
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
