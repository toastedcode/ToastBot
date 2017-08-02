#include <Arduino.h>
#include <Board.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <FS.h>
#include <Messaging.h>
#include <NewPing.h>
#include <Logger.h>
#include <Properties.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <ToastBot.h>
#include <WebSocketServer.h>

#include "Robox.h"

class ConfigPage : public Webpage
{

public:
  
  ConfigPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      MessagePtr message,
      String& responsePath);

protected:

  virtual void replaceContent(
     String& content);
};

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
  if (message->isSet("id"))
  {
    ToastBot::setProperty("deviceName", message->getString("id"));
  }

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
   

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());

   ToastBot::addComponent(new Robox(), true);  // <-- default handler

   Logger::setLogLevel(DEBUG_FINEST);

   WebServerAdapter* webServerAdapter = new WebServerAdapter("web", 80);
   ToastBot::addComponent(webServerAdapter);
   webServerAdapter->addPage(new ConfigPage());
}

void loop()
{
   ToastBot::loop();
}
