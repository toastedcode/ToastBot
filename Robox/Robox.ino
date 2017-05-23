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
   else if (message->getMessageId() == "reset")
   {
      Logger::logDebug("Resetting ...");
      Board::getBoard()->reset();
   }
   else if (message->getMessageId() == "property")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      if (propertyName != "")
      {
         Properties properties("/toastbot.properties");

         if (propertyValue != "")
         {
            properties.set(propertyName, propertyValue);
            Logger::logDebug("Updated properties: %s = %s", propertyName.c_str(), propertyValue.c_str());
         }
         else
         {
            properties.remove(propertyName);
            Logger::logDebug("Removed property: %s", propertyName.c_str());
         }

         properties.save();
      }
   }
   else if (message->getMessageId() == "digitalWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("digitalWrite(%d, %d)", pin, value);
      Board::getBoard()->digitalWrite(pin, value);
   }
   else if (message->getMessageId() == "analogWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("analogWrite(%d, %d)", pin, value);
      Board::getBoard()->analogWrite(pin, value);
   }
   else
   {
      Component::handleMessage(message);
   }
}

void setup()
{
   Serial.begin(9600);

   SPIFFS.begin();
    
   Logger::setLogger(new SerialLogger());

   Properties properties;
   properties.load("/toastbot.properties");
   Logger::logDebug("Properties: \n%s", properties.toString().c_str());

   WifiBoard* board = new Esp8266Board();
   Board::setBoard(board);

   // Connect to a known network.
   //if (board->connectWifi("NETGEAR69", "silentsky723", 15) == false)
   //if (board->connectWifi("Massive", "getshitdone", 15) == false)
   if (board->connectWifi("compunetix-guest", "compunetix", 15) == false)
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
   
   MotorPair* motorPair1 = new MotorPair("motorPair1", motor1, motor2);
   ToastBot::add(motorPair1);

   ServoComponent* servo1 = new ServoComponent("servo1", 14);
   ToastBot::add(servo1);
   ToastBot::add(new ServoComponent("servo2", 12));

   DistanceSensor* distance1 = new DistanceSensor("distance1", 13, 15, 200);
   ToastBot::add(distance1);

   /*
   Scanner* scanner1 = new Scanner("scanner1", servo1, distance1);
   ToastBot::add(scanner1);

   FollowAI* follow1 = new FollowAI("follow1", scanner1, motorPair1);
   ToastBot::add(follow1);

   FollowAI_2* follow2 = new FollowAI_2("follow2", distance1, motorPair1);
   ToastBot::add(follow2);
   */

   //ToastBot::add(new ScoutBehavior("scout1", motorPair1, distance1));

   ToastBot::add(new SerialAdapter("serial", new JsonProtocol()));
   //ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::add(new TcpServerAdapter("adapter2", new JsonProtocol(), 1975));
   //ToastBot::add(new MqttClientAdapter("adapter3", new JsonProtocol(), "broker.mqtt-dashboard.com", 1883, "toastbot1", "", ""));
   ToastBot::add(new UdpAdapter("adapter4", new JsonProtocol(), 1993));
   //ToastBot::add(new TcpClientAdapter("adapter5", new JsonProtocol(), "192.168.1.1", 1997));

   //Logger::setLogger(new RemoteLogger("adapter1"));

   ToastBot::setup("myMachine");
}

void loop()
{
   ToastBot::loop();
}
