#include "Connection.hpp"
#include "Logger.hpp"
#include "WifiBoard.hpp"

static const int WIFI_RETRY_TIME = 10;  // seconds

const ConnectionMode Connection::DEFAULT_MODE = OFFLINE;

ConnectionMode Connection::mode = DEFAULT_MODE;

WifiConfig Connection::accessPointConfig;

WifiConfig Connection::wifiConfig;

ServerConfig Connection::serverConfig;

Led* Connection::statusLed = 0;

ClientAdapter* Connection::onlineAdapter = 0;

// *****************************************************************************
//                                   Public
// *****************************************************************************

ConnectionMode Connection::getMode()
{
   return (mode);
}

void Connection::setMode(
   const ConnectionMode mode)
{
   Logger::logDebug(F("Connection::setMode: %s -> %s"), toString(Connection::mode).c_str(), toString(mode).c_str());

   if (Connection::mode != mode)
   {
      Connection::mode = mode;

      setup();
   }
}

WifiConfig Connection::getApConfig()
{
   return (accessPointConfig);
}

void Connection::setApConfig(
   const String& ssid,
   const String& password)
{
   accessPointConfig.ssid = ssid;
   accessPointConfig.password = password;

   if (mode == ACCESS_POINT)
   {
      setup();
   }
}

WifiConfig Connection::getWifiConfig()
{
   return (wifiConfig);
}

void Connection::setWifiConfig(
   const WifiConfig& wifiConfig)
{
   Connection::wifiConfig = wifiConfig;

   if ((mode == WIFI ) || (mode == ONLINE ))
   {
      setup();
   }
}

void Connection::setWifiConfig(
   const String& ssid,
   const String& password)
{
   wifiConfig.ssid = ssid;
   wifiConfig.password = password;

   if ((mode == WIFI ) || (mode == ONLINE ))
   {
      setup();
   }
}

ServerConfig Connection::getServerConfig()
{
   return (serverConfig);
}

void Connection::setServerConfig(
   const ServerConfig& serverConfig)
{
   Connection::serverConfig = serverConfig;

   if (mode == ONLINE)
   {
      setup();
   }
}

void Connection::setServerConfig(
   const String& host,
   const int& port,
   const String& userId,
   const String& password,
   const String& clientId,
   const String& topic)
{
   serverConfig.host = host;
   serverConfig.port = port;
   serverConfig.userId = userId;
   serverConfig.password = password;
   serverConfig.clientId = clientId;
   serverConfig.topic = topic;
}

void Connection::setStatusLed(
   Led* statusLed)
{
   Connection::statusLed = statusLed;

   updateStatusLed();
}

void Connection::setOnlineAdapter(
   ClientAdapter* onlineAdapter)
{
   Connection::onlineAdapter = onlineAdapter;
}

// *****************************************************************************
//                                   Private
// *****************************************************************************

void Connection::setup()
{
   WifiBoard* board = WifiBoard::getBoard();
   if (board)
   {
      breakdown();

      switch (getMode())
      {
         case OFFLINE:
         {
            // Nothing else to do here.
            break;
         }

         case ACCESS_POINT:
         {
            board->startAccessPoint(accessPointConfig.ssid, accessPointConfig.password);
            break;
         }

         case WIFI:
         {
            board->startAccessPoint(accessPointConfig.ssid, accessPointConfig.password);

            if (board->connectWifi(wifiConfig.ssid, wifiConfig.password, WIFI_RETRY_TIME) == false)  // blocks
            {
               Logger::logWarning(F("Connection::setup: Failed to connect WIFI."));
            }
            break;
         }

         case ONLINE:
         {
            board->startAccessPoint(accessPointConfig.ssid, accessPointConfig.password);

            if (board->connectWifi(wifiConfig.ssid, wifiConfig.password, WIFI_RETRY_TIME) == false)  // blocks
            {
               Logger::logWarning(F("Connection::setup: Failed to connect WIFI."));
            }
            else if ((onlineAdapter) && (onlineAdapter->connect() == false))
            {
               Logger::logWarning(F("Connection::setup: Failed to make online connection."));
            }
         }
      }
   }

   updateStatusLed();
}

void Connection::breakdown()
{
   WifiBoard* board = WifiBoard::getBoard();
   if (board)
   {
      board->stopAccessPoint();

      if (board->isConnected())
      {
         board->disconnectWifi();
      }

      if (onlineAdapter)
      {
         onlineAdapter->disconnect();
      }
   }
}

void Connection::updateStatusLed()
{
   if (statusLed)
   {
      switch (mode)
      {
         case OFFLINE:
         {
            statusLed->setBrightness(0);
            break;
         }
         
         case ACCESS_POINT:
         {
            statusLed->pulse(2000);  // 2-second pulse
            break;
         }
         
         case WIFI:
         {
            WifiBoard* board = WifiBoard::getBoard();
            if (board && board->isConnected())
            {
               statusLed->blink("_--------");  // 2-second blink
            }
            else
            {
               statusLed->pulse(2000);  // 2-second pulse
            }
            break;
         }
         
         case ONLINE:
         {
            WifiBoard* board = WifiBoard::getBoard();
            if (board && board->isConnected())
            {
               if (onlineAdapter && onlineAdapter->isConnected())
               {
                  statusLed->blink("_--_-----");  // 2-second, double blink
               }
               else
               {
                  statusLed->blink("_--------");  // 2-second blink
               }
            }
            else
            {
               statusLed->pulse(2000);  // 2-second pulse
            }
            break;

            break;
         }
      }
   }
}
