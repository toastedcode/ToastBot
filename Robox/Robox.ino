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

// TODO: Move to ToastBot
void createComponents()
{
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

   Led* led1 = new Led("led1", 16);
   ToastBot::add(led1);

   ToastBot::add(new SerialAdapter("serial", new JsonProtocol()));
   ToastBot::add(new UdpAdapter("discover", new JsonProtocol(), 1993));  
   ToastBot::add(new TcpServerAdapter("control", new JsonProtocol(), 1975));
   ToastBot::add(new TcpServerAdapter("debug", new JsonProtocol(), 1977));

   // Experimental components.   
   /*
   Scanner* scanner1 = new Scanner("scanner1", servo1, distance1);
   ToastBot::add(scanner1);

   FollowAI* follow1 = new FollowAI("follow1", scanner1, motorPair1);
   ToastBot::add(follow1);

   FollowAI_2* follow2 = new FollowAI_2("follow2", distance1, motorPair1);
   ToastBot::add(follow2);

   ToastBot::add(new ScoutBehavior("scout1", motorPair1, distance1));
   
   ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::add(new MqttClientAdapter("adapter3", new JsonProtocol(), "broker.mqtt-dashboard.com", 1883, "toastbot1", "", ""));
   ToastBot::add(new UdpAdapter("adapter5", new JsonProtocol(), IPAddress(10, 1, 11, 249), 55056));
   ToastBot::add(new TcpClientAdapter("adapter5", new JsonProtocol(), "10.1.11.249", 1997));
   */
}

// TODO: Move to ToastBot
void createConnections()
{
   const Properties& properties = ToastBot::getProperties();

   // Connect to a wifi network.
   String ssid = properties.getString("wifi.ssid");
   String password = properties.getString("wifi.password");
   String deviceName = properties.getString("deviceName");
   if ((ssid == "") ||
       (WifiBoard::getBoard()->connectWifi(ssid, password, 15) == false))
   {
      // If the ESP8266 fails to connect with the stored credentials, we'll create an AP to allow for wifi config.
      String apName = (deviceName.length() > 0) ? deviceName : Robox::getUniqueId();
      WifiBoard::getBoard()->startAccessPoint(apName, "");
   }
   
   // Blink the onboard LED to indicate connection status.
   Led* led = (Led*)ToastBot::get("led1");
   if (WifiBoard::getBoard()->isConnected())
   {
      led->blink("_--------");
   }
   else
   {
      led->pulse(2000);
   }
}

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());
   
   createComponents();
   
   createConnections();
}

void loop()
{
   ToastBot::loop();
}
