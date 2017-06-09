#include <FS.h>

#include "ConfigServer.hpp"
#include "EepromUtils.hpp"
#include "Logger.hpp"

// **************************************************************************
//                                Static members
// **************************************************************************

ESP8266WebServer ConfigServer::server(80);

DeviceConfig ConfigServer::deviceConfig;

WifiConfig ConfigServer::wifiConfig;

// **************************************************************************
//                                 Public
// **************************************************************************

void ConfigServer::begin()
{
   SPIFFS.begin();
  
   // Load config from EEPROM.
   deviceConfig = EepromUtils::getDeviceConfig();
   wifiConfig = EepromUtils::getWifiConfig();

   Logger::logDebug("Device name: %s", deviceConfig.deviceName);
   Logger::logDebug("Authorization code: %s", deviceConfig.authCode);
   Logger::logDebug("SSID: %s", wifiConfig.ssid);
   Logger::logDebug("Password: %s", wifiConfig.password);
  
   // Setup  handlers.
   server.on("/", ConfigServer::handleRoot);
   server.onNotFound(handleNotFound);
  
   server.begin();
  
  Logger::logDebug("HTTP server started.");
}

void ConfigServer::run()
{
   server.handleClient();
}

DeviceConfig ConfigServer::getDeviceConfig()
{
  return (deviceConfig);
}

WifiConfig ConfigServer::getWifiConfig()
{
   return (wifiConfig);
}

// **************************************************************************
//                                 Private
// **************************************************************************

void ConfigServer::handleRoot()
{
   String newDeviceName = server.arg("id");
   String newAuthCode = server.arg("authcode");
   String newSsid = server.arg("ssid");
   String newPassword = server.arg("password");

   bool deviceConfigChanged = false;
   bool wifiConfigChanged = false;

   if (server.hasArg("id") && 
       (strncmp(newDeviceName.c_str(), deviceConfig.deviceName, sizeof(deviceConfig.deviceName)) != 0))
   {
      strncpy(deviceConfig.deviceName, newDeviceName.c_str(), sizeof(deviceConfig.deviceName));   
      deviceConfigChanged = true;
   }

   if (server.hasArg("authcode") && 
       (strncmp(newAuthCode.c_str(), deviceConfig.authCode, sizeof(deviceConfig.authCode)) != 0))
   {
      strncpy(deviceConfig.authCode, newAuthCode.c_str(), sizeof(deviceConfig.authCode));    
      deviceConfigChanged = true;
   }

   if (deviceConfigChanged == true)
   {
      Logger::logDebug("Updating device config [%s, %s].", deviceConfig.deviceName, deviceConfig.authCode);
      EepromUtils::setDeviceConfig(deviceConfig);
   }
   
   if (server.hasArg("ssid") && 
       (strncmp(newSsid.c_str(), wifiConfig.ssid, sizeof(wifiConfig.ssid)) != 0))
   {
      strncpy(wifiConfig.ssid, newSsid.c_str(), sizeof(wifiConfig.ssid));      
      wifiConfigChanged = true;
   }

   if (server.hasArg("password") && 
       (strncmp(newSsid.c_str(), wifiConfig.password, sizeof(wifiConfig.password)) != 0))
   {
      strncpy(wifiConfig.password, newPassword.c_str(), sizeof(wifiConfig.password));   
      wifiConfigChanged = true;
   }
  
   if (wifiConfigChanged == true)
   {
      Logger::logDebug("Updating wifi config [%s, %s].", wifiConfig.ssid, wifiConfig.password);
      EepromUtils::setWifiConfig(wifiConfig);
   }
   
   File file = SPIFFS.open("/config.html", "r");
   if (!file)
   {
      Serial.println("file open failed");
      server.send(404, "text/plain", "File not found.");
   }
   else
   {
      Serial.println("Opened file.");

      //server.streamFile(file, "text/html");

      String content = file.readString();

      content.replace("%name", deviceConfig.deviceName);
      content.replace("%authcode", deviceConfig.authCode);
      content.replace("%ssid", wifiConfig.ssid);
      content.replace("%password", wifiConfig.password);

      server.send(200, "text/html", content);

      file.close();
   }

   if (deviceConfigChanged || wifiConfigChanged)
   {
      Logger::logDebug("Resetting to apply config changes.");

      ESP.restart();
   }
}

void ConfigServer::handleNotFound()
{
   server.send(404, "text/plain", "File not found.");
}
