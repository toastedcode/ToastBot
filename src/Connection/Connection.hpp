#pragma once

#include  "../Adapter/ClientAdapter.hpp"
#include "../Component/Led.hpp"
#include "ConnectionDefs.hpp"

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
      const WifiConfig& wifiConfig);

   static void setWifiConfig(
      const String& ssid,
      const String& password);

   static ServerConfig getServerConfig();

   static void setServerConfig(
      const ServerConfig& serverConfig);

   static void setServerConfig(
      const String& host,
      const int& port,
      const String& userId,
      const String& password,
      const String& clientId,
      const String& topic);

   static void setOnlineAdapter(
      ClientAdapter* onlineAdapter);

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

   static ClientAdapter* onlineAdapter;
};
