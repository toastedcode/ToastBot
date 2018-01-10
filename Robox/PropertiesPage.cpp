#include <FS.h>

#include "Board.hpp"
#include "CommonDefs.hpp"
#include "Logger.hpp"
#include "Properties.hpp"
#include "PropertiesPage.hpp"
#include "ToastBot.hpp"

PropertiesPage::PropertiesPage() :
   Webpage("/properties.html",
           "/properties.html")
{
   // Nothing to do here.  
}

PropertiesPage::~PropertiesPage()
{
   // Nothing to do here.
}

bool PropertiesPage::handle(
   const HTTPMethod& requestMethod,
   const String& requestUri,
   const Dictionary& arguments,
   String& responsePath)
{
   bool success = false;
   responsePath = "";

   if (canHandle(requestMethod, requestUri))
   {
       Properties& properties = ToastBot::getProperties();

       String action = arguments.getString("action");
    
       if ((action == "add") ||
           (action == "update"))
       {
          String propertyName = arguments.getString("propertyName");
          String propertyValue = arguments.getString("propertyValue");
    
          if (propertyName != "")
          {
             properties.set(propertyName, propertyValue);
             properties.save();
          }
       }
       else if (action == "delete")
       {
          String propertyName = arguments.getString("propertyName");
          
          properties.erase(propertyName);
          properties.save();
       }
       else if (action == "reset")
       {
          Board::getBoard()->reset();
       }
   
      if (getPath() != "")
      {
         responsePath = "/tmp" + getPath();

         File file = SPIFFS.open(getPath(), "r");
         File responseFile = SPIFFS.open(responsePath, "w");

         if (!file)
         {
            Logger::logWarning(F("Webpage::handle: Failed to load webpage file: %s"), getPath().c_str());
            responsePath = "";
         }
         else if (!responseFile)
         {
            Logger::logWarning(F("Webpage::handle: Failed to create temp file: %s"), getPath().c_str());
            responsePath = "";
         }
         else
         {
            // Line by line, read from the template file and create the content file.
            String line = file.readStringUntil('\n');
            while (line.length() > 0)
            {
               // Look for the %properties tag and replace.
               if (line.indexOf("%properties") != -1)
               {
                  for (Properties::Iterator it = properties.begin(); it != properties.end(); it++)
                  {
                     String propertyDiv = "";
                     getPropertyDiv(it->first, it->second, propertyDiv);   
                     responseFile.println(propertyDiv);   
                  }
               }
               else
               {
                  responseFile.println(line);
               }

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

void PropertiesPage::getPropertyDiv(
   const String& propertyName,
   const String& propertyValue,
   String& content)
{
   String divTemplate = 
      "<div class=\"flex-row card property-card\">"
      "<input type=\"text\" value=\"%propertyName\" disabled></input>"
      "<input id=\"%propertyName\" type=\"text\" name=\"%propertyName\"value=\"%propertyValue\"></input>"
      "<div class=\"update-button\" onclick=\"updateProperty('%propertyName')\"><img class=\"action-icon\" src=\"check.png\"/></div>"
      "<div class=\"delete-button\" onclick=\"deleteProperty('%propertyName')\"><img class=\"action-icon\" src=\"close.png\"/></div>"
      "</div>";

   divTemplate.replace("%propertyName", propertyName);
   divTemplate.replace("%propertyValue", propertyValue);

   content += divTemplate;
}
