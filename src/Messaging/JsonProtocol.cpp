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

   // Strip braces, quotes, and whitespace.
   remainingString = StringUtils::removeAll(remainingString, " \n\r\t{}\"\'");

   // Parse parameters.
   if (parseParameters(remainingString, parameters) == false)
   {
      // TODO: Logging.
      printf("Failed to parse parameters.\n");
   }
   else
   {
      // Populate message with parsed parameters.
      for (int i = 0; i < parameters.length(); i++)
      {
         Parameter parameter = parameters.item(i)->value;

         message->set(parameter.name, parameter.value);
      }

      isSuccess = true;
   }
}

String JsonProtocol::serialize(
   MessagePtr message) const
{
   return ("");
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
      parameter.name = name;
      parameter.value = value;

      isSuccess = true;
   }
   else
   {
      // TODO: Logging.
      printf("Failed to parse parameter: %s\n", parameterString.c_str());
   }

   return (isSuccess);
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
