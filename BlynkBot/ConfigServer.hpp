#pragma once

#include <ESP8266WebServer.h>

static const int DEVICE_NAME_SIZE = 20;

static const int AUTH_CODE_SIZE = 20;

static const int SSID_SIZE = 20;

static const int PASSWORD_SIZE = 20;

struct DeviceConfig
{
  char deviceName[DEVICE_NAME_SIZE];

  char authCode[AUTH_CODE_SIZE];
};

struct WifiConfig
{
   char ssid[SSID_SIZE];

   char password[PASSWORD_SIZE];
};

class ConfigServer
{
  
public:  

   static void begin();

   static void run();

   static DeviceConfig getDeviceConfig();

   static void setDeviceConfig(
      const DeviceConfig& deviceConfig);
    
   static WifiConfig getWifiConfig();

   static void setWifiConfig(
      const WifiConfig& wifiConfig);

private:

   static void handleRoot();

   static void handleNotFound();

   static ESP8266WebServer server;

   static DeviceConfig deviceConfig;

   static WifiConfig wifiConfig;
};

