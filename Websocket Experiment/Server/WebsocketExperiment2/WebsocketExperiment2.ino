#include <Arduino.h>
#include <ESP8266.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <Messaging.h>
#include <SerialLogger.h>
#include <ToastBot.h>
#include <WebSocketServer.h>

class Robox : public Component
{
  
public:
  
   // Constructor.
   inline Robox() :
      Component("robox") {}

   // Destructor.
   inline virtual ~Robox() {}

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);
};

void Robox::handleMessage(
      MessagePtr message)
{
   //  ping
   if (message->getMessageId() == "ping")
   {
      Message* reply = Messaging::newMessage();
      
      reply->setMessageId("pong");
      reply->setSource(getId());
      reply->setDestination(message->getSource());
      reply->set("deviceId", ToastBot::getId());
      reply->set("ipAddress", Esp8266::getInstance()->getIpAddress().toString().c_str());
      
      Messaging::send(reply);

      message->setFree();   
   }
   else
   {
      Component::handleMessage(message);
   }
}

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

   Logger::logDebug("Starting TOASTBOT");

   ToastBot::add(new Robox(), true);// <-- default handler

   Motor* motor1 = new Motor("motor1", 0, 5);
   Motor* motor2 = new Motor("motor2", 2, 4);
   
   ToastBot::add(motor1);
   ToastBot::add(motor2);
   ToastBot::add(new MotorPair("motorPair1", motor1, motor2));

   ToastBot::add(new ServoComponent("servo1", 6));  // TODO: Pick appropriate pin.
   
   ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::add(new IpServerAdapter("adapter2", new JsonProtocol(), 80));

   ToastBot::setup("myMachine");
}

void loop()
{
   ToastBot::loop();
}
