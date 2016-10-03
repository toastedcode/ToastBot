#include <Arduino.h>
#include <ESP8266.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <Messaging.h>
#include <SerialLogger.h>
#include <ToastBot.h>
#include <WebSocketServer.h>

class MyComponent : public Component
{

public:
  
   MyComponent(String id) : Component(id) {}

   virtual void handleMessage(MessagePtr message)
   {
      // Format: {"variable1":"", "variable2":""}
      Logger::logDebug("Received message: " + 
                       message->getMessageId() + 
                       " (variable1 = " + 
                       message->getString("variable1") + 
                       ")");
   }
};

void setup()
{
   Serial.begin(9600);
    
   Logger::setLogger(new SerialLogger());
   Logger::setEnabled(true);
   
   // Connect to a network via the ESP8266 wifi adapter.
   //if (Esp8266::getInstance()->connectWifi("NETGEAR69", "silentsky723", 15) == false)
   if (Esp8266::getInstance()->connectWifi("compunetix-guest", "compunetix", 15) == false)
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      Esp8266::getInstance()->startAccessPoint("TOASTBOT", "");
   } 
  
   ToastBot::setup("myMachine");

   ToastBot::add(new MyComponent("component1"));

   ToastBot::add(new Motor("motor1", 1, 2));

   ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol()));

   ToastBot::add(new IpServerAdapter("adapter2", new JsonProtocol(), 80));

   ToastBot::add(new IpClientAdapter("adapter3", new JsonProtocol(), "10.4.41.179", 5000));
}

void loop()
{
   ToastBot::loop();
}
