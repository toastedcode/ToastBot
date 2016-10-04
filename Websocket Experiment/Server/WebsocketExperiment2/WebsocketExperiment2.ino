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
      Logger::logDebug("Received message: " + message->getMessageId() + "\n");

      if (message->getMessageId() == "ping")
      {
         Logger::logDebug("Received: ping\n");
        
         MessagePtr reply = Messaging::newMessage();
         if (reply)
         {
            reply->setMessageId("pong");
            reply->setDestination(message->getSource());
            reply->setSource(getAddress());
            Messaging::send(reply); 
         }
      }
      else
      {
        Logger::logDebug("Unhandled message: " + message->getMessageId() + "\n");
      }
   }
};

void setup()
{
   Serial.begin(9600);
    
   Logger::setLogger(new SerialLogger());
   Logger::setEnabled(true);
   
   // Connect to a network via the ESP8266 wifi adapter.
   if (Esp8266::getInstance()->connectWifi("NETGEAR69", "silentsky723", 15) == false)
   //if (Esp8266::getInstance()->connectWifi("compunetix-guest", "compunetix", 15) == false)
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      Esp8266::getInstance()->startAccessPoint("TOASTBOT", "");
   } 

   ToastBot::setup("myMachine");

   ToastBot::add(new MyComponent("component1"));

   Motor* motor1 = new Motor("motor1", 1, 2);
   Motor* motor2 = new Motor("motor2", 3, 4);
   
   ToastBot::add(motor1);
   ToastBot::add(motor2);
   ToastBot::add(new MotorPair("motorPair", motor1, motor2));
   
   //ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol()));

   // TODO: Must call ToastBot::setup() after register, must must setup MessageRouter with id before adding.  Catch-22!
   ToastBot::add(new IpServerAdapter("adapter2", new JsonProtocol(), 80));

   //ToastBot::add(new IpClientAdapter("adapter3", new JsonProtocol(), "10.4.41.179", 5000));
}

void loop()
{
   ToastBot::loop();
}
