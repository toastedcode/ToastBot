#include <Arduino.h>
#include <ESP8266.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <Messaging.h>
#include <SerialLogger.h>
#include <ToastBot.h>
#include <WebSocketServer.h>

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

   Motor* motor1 = new Motor("motor1", 5, 4);
   Motor* motor2 = new Motor("motor2", 3, 2);
   
   ToastBot::add(motor1);
   ToastBot::add(motor2);
   ToastBot::add(new MotorPair("motorPair1", motor1, motor2), true);  // <-- default handler
   
   ToastBot::add(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::add(new IpServerAdapter("adapter2", new JsonProtocol(), 80));

   ToastBot::setup("myMachine");
}

void loop()
{
   ToastBot::loop();
}
