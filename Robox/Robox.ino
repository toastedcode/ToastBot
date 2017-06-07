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

   // Connect to a wifi network.
   String ssid = properties.getString("wifi.ssid");
   String password = properties.getString("wifi.password");
   String deviceName = properties.getString("deviceName");
   if ((ssid == "") ||
       (board->connectWifi(ssid, password, 15) == false))
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      String apName = (deviceName.length() > 0) ? deviceName : Robox::getUniqueId();
      board->startAccessPoint(apName, "");
   }

   Logger::logDebug("Starting ROBOX");

   ToastBot::add(new Robox(), true);  // <-- default handler

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
   //ToastBot::add(new UdpAdapter("adapter5", new JsonProtocol(), IPAddress(10, 1, 11, 249), 55056));
   //ToastBot::add(new TcpClientAdapter("adapter5", new JsonProtocol(), "10.1.11.249", 1997));

   //Logger::setLogger(new RemoteLogger("adapter2"));

   ToastBot::setup(properties.getString("deviceName"));
}

void loop()
{
   ToastBot::loop();
}
