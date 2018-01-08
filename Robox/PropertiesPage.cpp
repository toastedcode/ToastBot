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

   return (Webpage::handle(requestMethod, requestUri, arguments, responsePath));
}

void PropertiesPage::replaceContent(
   String& content)
{
  String propertiesContent = "";

  Properties& properties = ToastBot::getProperties();

  for (Properties::Iterator it = properties.begin(); it != properties.end(); it++)
  {
     getPropertyDiv(it->first, it->second, propertiesContent);
  }

  content.replace("%properties", propertiesContent);
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
