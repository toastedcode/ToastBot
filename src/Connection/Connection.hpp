#pragma once

#include "ConnectionDefs.hpp"
#include "../Component/Led.hpp"

class Connection
{

public:

   static ConnectionMode getMode();

   static void setMode(
      const ConnectionMode mode);

   static void setup();

   static WifiConfig getApConfig();

   static void setApConfig(
      const String& ssid,
      const String& password);

   static WifiConfig getWifiConfig();

   static void setWifiConfig(
      const String& ssid,
      const String& password);

   static ServerConfig getServerConfig();

   static void setServerConfig(
      const String& host,
      const String& clientId,
      const String& clientPassword);

   static void setStatusLed(
      Led* statusLed);

private:

   static void updateStatusLed();

   static const ConnectionMode DEFAULT_MODE;

   static ConnectionMode mode;

   static WifiConfig accessPointConfig;

   static WifiConfig wifiConfig;

   static ServerConfig serverConfig;

   static Led* statusLed;
};
