#include "JsonProtocol.hpp"
#include "StringUtils.hpp"

const int JsonProtocol::MAX_PARAMETERS;

bool JsonProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   bool isSuccess = false;

   String remainingString = messageString;
   ParameterSet parameters;

#ifdef MESSAGING_DEBUG
   printf("JsonProtocol::parse: Parsing: \"%s\".\n", messageString.c_str());
#endif

   // Strip braces and whitespace.
   remainingString = StringUtils::removeAll(remainingString, " \n\r\t{}");

   // Parse parameters.
   if (parseParameters(remainingString, parameters) == false)
   {
#ifdef MESSAGING_DEBUG
      printf("JsonProtocol::parse: Failed to parse parameters.\n");
#endif
   }
   else
   {
      // Populate message with parsed parameters.
      for (int i = 0; i < parameters.length(); i++)
      {
         Parameter parameter = parameters.item(i)->value;

         message->setParameter(parameter);
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

   for (int i = 0; i < message->getParameterCount(); i++)
   {
      Parameter parameter = message->getParameter(i);

      String parameterValue = parameter.value;

      if ((parameter.type == STRING) ||
          (parameter.type == CHAR))
      {
         parameterValue = wrap(parameterValue, '\"');
      }

      serializedParameters += (wrap(parameter.name, '\"') + ":" + parameterValue);

      if (i < (message->getParameterCount() - 1))
      {
         serializedParameters += ", ";
      }
   }

   return (serializedParameters);
}

bool JsonProtocol::parseParameters(
   const String& parameterString,
   ParameterSet& parameters)
{
   bool isSuccess = true;

   String remainingString = parameterString;
   Parameter parameter;

   while (isSuccess && (remainingString != ""))
   {
      isSuccess = parseParameter(StringUtils::tokenize(remainingString, ","), parameter);

      if (isSuccess)
      {
         parameters.add(parameter);
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
      parameter.type = getType(value);
      parameter.name = stripQuotes(name);
      parameter.value = stripQuotes(value);
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

ParameterType JsonProtocol::getType(
   const String& value)
{
   ParameterType type = STRING;

   String lowerCase = value;
   lowerCase.toLowerCase();

   if ((value.charAt(0) == '\"') &&
       (value.charAt(value.length() - 1) == '\"'))
   {
      if (value.length() == 3)
      {
         type = CHAR;
      }
      else
      {
         type = STRING;
      }
   }
   else if (StringUtils::findFirstNotOf(value, "123456789.-") == -1)
   {
      // TODO: Parse numeric types.
      type = INT;
   }
   else if ((lowerCase == "true") ||
            (lowerCase == "false"))
   {
      type = BOOL;
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
