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
};

struct ServerConfig
{
   String host;

   int port;

   String userId;

   String password;

   String clientId;

   inline ServerConfig() :
      host(""),
      port(0),
      userId(""),
      password(""),
      clientId("")
   {
   }

   inline ServerConfig(
      const String& host,
      const int& port,
      const String& userId,
      const String& password,
      const String& clientId) :
         host(host),
         port(port),
         userId(userId),
         password(password),
         clientId(clientId)
   {
   }
};
