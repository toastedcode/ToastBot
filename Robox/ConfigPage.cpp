#include "ConfigPage.hpp"
#include "Connection.h"
#include "ToastBot.hpp"

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
   
   //
   // Update server config.
   //
      
   ServerConfig serverConfig(message->getString("server.host"),
                             message->getString("server.port").toInt(),
                             message->getString("server.userId"),
                             message->getString("server.password"),
                             Connection::getServerConfig().clientId,  // TODO
                             message->getString("server.topic"));
                             
   Connection::setServerConfig(serverConfig);
   ToastBot::setProperty("server.host", serverConfig.host);
   ToastBot::setProperty("server.port", serverConfig.port);
   ToastBot::setProperty("server.userId", serverConfig.userId);
   ToastBot::setProperty("server.password", serverConfig.password);
   ToastBot::setProperty("server.topic", serverConfig.topic);
   
   Connection::setup();

   return (Webpage::handle(requestMethod, requestUri, message, responsePath));    
   return (true);
}

void ConfigPage::replaceContent(
   String& content)
{
  content.replace("%name", ToastBot::getId());
  content.replace("%ssid", Connection::getWifiConfig().ssid);
  content.replace("%password", Connection::getWifiConfig().password);
  content.replace("%server.host", Connection::getServerConfig().host);
  content.replace("%server.port", String(Connection::getServerConfig().port));
  content.replace("%server.userId", Connection::getServerConfig().userId);
  content.replace("%server.password", Connection::getServerConfig().password);
  content.replace("%server.topic", Connection::getServerConfig().topic);
  content.replace("%info", "Successfully updated.");
}
