#pragma once

#include "CommonDefs.hpp"

enum ConnectionMode
{
   CONNECTION_MODE_FIRST,
   OFFLINE = CONNECTION_MODE_FIRST,
   ACCESS_POINT,
   WIFI,
   ONLINE,
   CONNECTION_MODE_LAST,
};

inline String toString(
   const ConnectionMode& mode)
{
   static const String enumToString[]
   {
      "OFFLINE",
      "ACCESS_POINT",
      "WIFI",
      "ONLINE",
   };

   return (enumToString[mode]);
}

inline ConnectionMode parseConnectionMode(
   const String& modeString)
{
   ConnectionMode mode = ACCESS_POINT;

   for (int i = CONNECTION_MODE_FIRST; i < CONNECTION_MODE_LAST; i++)
   {
      if (modeString == toString(static_cast<ConnectionMode>(i)))
      {
         mode = static_cast<ConnectionMode>(i);
         break;
      }
   }

   return (mode);
}

struct WifiConfig
{
   String ssid;

   String password;

   inline WifiConfig() :
      ssid(""),
      password("")
   {
   }

   inline WifiConfig(
      const String& ssid,
      const String& password) :
      ssid(ssid),
      password(password)
   {
   }

   inline WifiConfig(
      const WifiConfig& wifiConfig)
   {
      ssid = wifiConfig.ssid;
      password = wifiConfig.password;
   }

   inline WifiConfig& operator=(
      const WifiConfig& rhs)
   {
      if (this != &rhs)
      {
         ssid = rhs.ssid;
         password = rhs.password;
      }

      return (*this);
   }
};

struct ServerConfig
{
   String host;

   int port;

   String userId;

   String password;

   String clientId;

   String topic;

   inline ServerConfig() :
      host(""),
      port(0),
      userId(""),
      password(""),
      clientId(""),
      topic("")
   {
   }

   inline ServerConfig(
      const String& host,
      const int& port,
      const String& userId,
      const String& password,
      const String& clientId,
      const String& topic) :
         host(host),
         port(port),
         userId(userId),
         password(password),
         clientId(clientId),
         topic(topic)
   {
   }

   inline ServerConfig(
      const ServerConfig& serverConfig)
   {
      host = serverConfig.host;
      port = serverConfig.port;
      userId = serverConfig.userId;
      password = serverConfig.password;
      clientId = serverConfig.clientId;
      topic = serverConfig.topic;
   }

   inline ServerConfig& operator=(
      const ServerConfig& rhs)
   {
      if (this != &rhs)
      {
         host = rhs.host;
         port = rhs.port;
         userId = rhs.userId;
         password = rhs.password;
         clientId = rhs.clientId;
         topic = rhs.topic;
      }

      return (*this);
   }
};
