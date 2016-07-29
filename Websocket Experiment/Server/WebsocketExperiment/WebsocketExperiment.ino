#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <Logger.h>
#include <SerialLogger.h>
#include <Utility.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(
   uint8_t num, 
   WStype_t type, 
   uint8_t * payload, 
   size_t lenght)
{
    switch(type)
    {
       case WStype_DISCONNECTED:
       {
          Logger::logDebug("Web socket disconnected\n");
          break;
       }
            
       case WStype_CONNECTED:
       {
          Logger::logDebug("Web socket connected\n");
          IPAddress ip = webSocket.remoteIP(num);
          break;
       }

       case WStype_TEXT:
       {
          String text = String((char *) &payload[0]);
          Logger::logDebug("Data received: " + text + "\n");
          
          // Parse the string into the JSON tree.
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& json = jsonBuffer.parseObject(text.c_str());
          
         if ((json.success() == false) ||
             (json.containsKey("messageId") == false))
         {
            Logger::logDebug("Bad JSON format.\n");
         }
         else
         {
            String messageId = json.get("messageId");
             
            if (messageId == "digitalWrite")
            {
               int pin = json.get("pin");
               int value = json.get("value");
                
               Logger::logDebug("digitalWrite(" + String(pin) + ", " + String(value) + ")\n");
                
               digitalWrite(pin, value);
            }
            else if (messageId == "digitalRead")
            {
               int pin = json.get("pin");
               
               Logger::logDebug("digitalRead(" + String(pin) + ")\n");
                
               // Create the JSON object.
               JsonObject& json = jsonBuffer.createObject();

               // Add fields.
               json["pin"] = pin;
               json["value"] = digitalRead(pin);

               // Serialize to string.
               String jsonString;
               json.printTo(jsonString);
                
               // Send across web socket.
               webSocket.sendTXT(num, jsonString);
            }
            else if (messageId == "pinMode")
            {
               int pin = json.get("pin");
               int mode = json.get("mode");
               
               Logger::logDebug("pinMode(" + String(pin) + ", " + String(mode) + ")\n");

               pinMode(pin, mode);
            }            
            else if (messageId == "motor")
            {
               String motor = json.get("motor");
               int deltaX = json.get("deltaX");
               int deltaY = json.get("deltaX");
               
               Logger::logDebug("motor(" + motor + ", " + String(deltaX) + ", " + String(deltaY) + ")\n");
            }                        
         }
      }
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

   webSocket.begin();
   webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
}
