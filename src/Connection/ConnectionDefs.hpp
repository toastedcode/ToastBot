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

   String clientId;

   String clientPassword;

   inline ServerConfig() :
      host(""),
      clientId(""),
      clientPassword("")
   {
   }

   inline ServerConfig(
      const String& host,
      const String& clientId,
      const String& clientPassword) :
      host(host),
      clientId(clientId),
      clientPassword(clientPassword)
   {
   }
};
