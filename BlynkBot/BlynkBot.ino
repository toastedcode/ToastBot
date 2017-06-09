#include <algorithm>
#include <Arduino.h>
#include <Common.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <Logger.h>

#include "ConfigServer.hpp"
#include "EepromUtils.hpp"

const String WIFI_AP_SSID = "ROBOXES_AP";

const int CONNECTION_TIMEOUT = 10;

void setupWifi()
{
   WifiConfig wifiConfig = ConfigServer::getWifiConfig();
   
   Logger::logDebug("Connecting to Wifi network %s ...", wifiConfig.ssid);

   WiFi.mode(WIFI_STA);
   WiFi.begin(wifiConfig.ssid, wifiConfig.password);

   int secondsToConnect = 0;

   // Wait until the connection is made, or until we timeout.
   while ((WiFi.status() != WL_CONNECTED)&&
          (secondsToConnect < CONNECTION_TIMEOUT))
   {
      delay(1000);
      secondsToConnect++;
      Logger::logDebug("trying ...");
   }

   if (WiFi.status() == WL_CONNECTED)
   {
      Logger::logDebug("Success!  Connected to %s at %s", wifiConfig.ssid, WiFi.localIP().toString().c_str());
   }
   else
   {
      Logger::logDebug("Failure!  Could not connect to %s", wifiConfig.ssid);

      WiFi.mode(WIFI_AP);
      WiFi.softAP(WIFI_AP_SSID.c_str(), "");
   }
}

void setup()
{
   Serial.begin(9600);

   Logger::setLogger(new SerialLogger());

   EepromUtils::begin();

   ConfigServer::begin();

   setupWifi();
}

void loop()
{
   ConfigServer::run();
}

