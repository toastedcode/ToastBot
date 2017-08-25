#include <FS.h>

#include "Logger.hpp"
#include "Webpage.hpp"

Webpage::Webpage(
   const String& uri) :
      uri(uri)
{

}

Webpage::Webpage(
   const String& uri,
   const String& path) :
      uri(uri),
      path(path)
{

}

Webpage::~Webpage()
{

}

String Webpage::getUri() const
{
   return (uri);
}

String Webpage::getPath() const
{
   return (path);
}

bool Webpage::canHandle(
   const HTTPMethod& requestMethod,
   const String& requestUri) const
{
   return (this->uri == uri);
}

bool Webpage::handle(
   const HTTPMethod& requestMethod,
   const String& requestUri,
   MessagePtr message,
   String& responsePath)
{
   bool success = false;
   responsePath = "";

   if (canHandle(requestMethod, requestUri))
   {
      if (path != "")
      {
         responsePath = "/tmp" + path;

         File file = SPIFFS.open(path, "r");
         File responseFile = SPIFFS.open(responsePath, "w");

         if (!file)
         {
            Logger::logWarning(F("Webpage::handle: Failed to load webpage file: %s"), path.c_str());
            responsePath = "";
         }
         else if (!responseFile)
         {
            Logger::logWarning(F("Webpage::handle: Failed to create temp file: %s"), path.c_str());
            responsePath = "";
         }
         else
         {
            // Line by line, read from the template file and create the content file.
            String line = file.readStringUntil('\n');
            while (line.length() > 0)
            {
               replaceContent(line);

               responseFile.println(line);

               line = file.readStringUntil('\n');
            }

            success = true;
         }

         file.close();
         responseFile.close();
      }
   }

   return (success);
}

void Webpage::replaceContent(
   String& content)
{
   // Override in child classes.
}
