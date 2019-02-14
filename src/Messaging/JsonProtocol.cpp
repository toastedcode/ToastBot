#include "JsonProtocol.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"

bool JsonProtocol::parse(
   const String& messageString,
   MessagePtr message)
{
   bool isSuccess = false;

   String remainingString = messageString;

   Logger::logDebugFinest(F("JsonProtocol::parse: Parsing: \"%s\"."), messageString.c_str());

   // Strip braces and whitespace.
   remainingString = StringUtils::removeAll(remainingString, " \n\r\t{}");

   // Parse parameters.
   if (parseParameters(remainingString, message) == false)
   {
      Logger::logWarning(F("JsonProtocol::parse: Failed to parse parameters."));
   }
   else
   {
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

   int paramIndex = 0;

   for (Message::Iterator it = message->begin(); it != message->end(); it++, paramIndex++)
   {
      String paramName = it->first;
      String paramValue = it->second;

      String lowerCase = paramValue;
      lowerCase.toLowerCase();

      // Determine if the string is numeric.
      bool isNumeric = true;
      int periodCount = 0;
      int dashCount = 0;
      for (int i = 0; i < paramValue.length(); i++)
      {
         char c = paramValue.charAt(i);

         if (c == '.')
         {
            periodCount++;
         }
         else if (c == '-')
         {
            dashCount++;
            isNumeric &= (i == 0);
         }
         else
         {
            isNumeric &= isDigit(c);
         }
      }
      isNumeric &= ((periodCount <= 1) && (dashCount <= 1));

      if (isNumeric)
      {
         // Numeric type.  No extra processing.
      }
      else if ((lowerCase == "true") ||
               (lowerCase == "false"))
      {
         // Bool type.  No extra processing.
      }
      else
      {
         // String type.  Wrap in quotes.
         paramValue = wrap(paramValue, '\"');
      }

      serializedParameters += (wrap(paramName, '\"') + ":" + paramValue);

      if (paramIndex < (message->size() - 1))
      {
         serializedParameters += ", ";
      }
   }

   return (serializedParameters);
}

bool JsonProtocol::parseParameters(
   const String& parameterString,
   MessagePtr message)
{
   bool isSuccess = true;

   String remainingString = parameterString;

   while (isSuccess && (remainingString != ""))
   {
      isSuccess = parseParameter(StringUtils::tokenize(remainingString, ","), message);
   }

   return (isSuccess);
}

bool JsonProtocol::parseParameter(
   const String& parameterString,
   MessagePtr message)
{
   bool isSuccess = false;

   String remainingString = parameterString;

   String paramName = StringUtils::tokenize(remainingString, ":");
   String paramValue = remainingString;

   if ((paramName != "") && (paramValue != ""))
   {
      message->set(stripQuotes(paramName), stripQuotes(paramValue));

      isSuccess = true;
   }
   else
   {
      Logger::logWarning(F("JsonProtocol::parse: Failed to parse parameter: \"%s\"."), parameterString.c_str());
   }

   return (isSuccess);
}

inline String JsonProtocol::stripQuotes(
   const String& value)
{
   return (StringUtils::removeAll(value, "\"\'"));
}
