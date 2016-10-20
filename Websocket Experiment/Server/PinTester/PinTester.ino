#include <Arduino.h>
#include <ESP8266.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <Messaging.h>
#include <Logger.h>
#include <PubSubClient.h>
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
   else if (message->getMessageId() == "digitalWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");
      Logger::logDebug("digitalWrite(%d, %d)", pin, value);

      pinMode(pin, OUTPUT);
      digitalWrite(pin, value);
      
      message->setFree(); 
   }
   else if (message->getMessageId() == "analogWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");
      Logger::logDebug("analogWrite(%d, %d)", pin, value);

      pinMode(pin, OUTPUT);
      analogWrite(pin, value);

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
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      Esp8266::getInstance()->startAccessPoint("TOASTBOT", "");
   } 

   Logger::logDebug("Starting Pin Tester");

   ToastBot::add(new Robox(), true);// <-- default handler
   
   //ToastBot::add(new MqttClientAdapter("adapter3", new JsonProtocol(), "broker.mqtt-dashboard.com", 1883, "toastbot1", "", ""));
   ToastBot::add(new MqttClientAdapter("adapter3", new JsonProtocol(), "test.mosquitto.org", 1883, "toastbot1", "", ""));

   ToastBot::setup("pinTester");
}

void loop()
{
   ToastBot::loop();
}
