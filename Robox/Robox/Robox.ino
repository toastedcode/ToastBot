#include <Arduino.h>
#include <Board.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <Messaging.h>
#include <Logger.h>
#include <PubSubClient.h>
#include <Timer.h>
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

      if (WifiBoard::getBoard())
      {
        reply->set("macAddress", WifiBoard::getBoard()->getMacAddress());
        reply->set("ipAddress", WifiBoard::getBoard()->getIpAddress());
      }
      
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

   WifiBoard* board = new Esp8266Board();
   Board::setBoard(board);
   
   // Connect to a known network.
   //if (board->connectWifi("NETGEAR69", "silentsky723", 15) == false)
   //if (board->connectWifi("compunetix-guest", "compunetix", 15) == false)
   if (board->connectWifi("Massive", "getshitdone", 15) == false)
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      board->startAccessPoint("TOASTBOT", "");
   }

   Logger::logDebug("Starting TOASTBOT");

   ToastBot::add(new Robox(), true);// <-- default handler

   Motor* motor1 = new Motor("motor1", 0, 5);
   Motor* motor2 = new Motor("motor2", 2, 4);
   
   ToastBot::add(motor1);
   ToastBot::add(motor2);
   ToastBot::add(new MotorPair("motorPair1", motor1, motor2));

   ToastBot::add(new ServoComponent("servo1", 14));
   ToastBot::add(new ServoComponent("servo2", 12));
   
   ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::add(new IpServerAdapter("adapter2", new JsonProtocol(), 80));
   ToastBot::add(new MqttClientAdapter("adapter3", new JsonProtocol(), "broker.mqtt-dashboard.com", 1883, "toastbot1", "", ""));

   ToastBot::setup("myMachine");
}

void loop()
{
   ToastBot::loop();
}
