#include "ConfigPage.h"

ConfigPage::ConfigPage() :
   Webpage("/config.html",
           "/config.html")
{
   // Nothing to do here.  
}

 bool ConfigPage::handle(
    const HTTPMethod& requestMethod,
    const String& requestUri,
    MessagePtr message,
    String& responsePath)
{
   // Update device config. 
   ToastBot::setProperty("deviceName", message->getString("id"));
   
   //
   // Update wifi config.
   //
   
   WifiConfig wifiConfig(message->getString("wifi.ssid"), message->getString("wifi.password"));
   
   Connection::setWifiConfig(wifiConfig);
   ToastBot::setProperty("wifi.ssid", wifiConfig.ssid);
   ToastBot::setProperty("wifi.ssid", wifiConfig.password);

  return (Webpage::handle(requestMethod, requestUri, message, responsePath));    
}

void ConfigPage::replaceContent(
   String& content)
{
  content.replace("%name", ToastBot::getId());
  content.replace("%ssid", Connection::getWifiConfig().ssid);
  content.replace("%password", Connection::getWifiConfig().password);
  content.replace("%info", "Successfully updated.");
}
