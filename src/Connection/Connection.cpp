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
   }

   setup();
}

void Connection::setup()
{
   WifiBoard* board = WifiBoard::getBoard();
   if (board)
   {
      switch (getMode())
      {
         case OFFLINE:
         {
            board->stopAccessPoint();
            if (board->isConnected())
            {
               board->disconnectWifi();
            }
            break;
         }
         
         case ACCESS_POINT:
         {
            board->stopAccessPoint();
            if (board->isConnected())
            {
               board->disconnectWifi();
            }
            
            board->startAccessPoint(accessPointConfig.ssid, accessPointConfig.password);
            break;
         }
         
         case WIFI:
         {
            board->stopAccessPoint();
            if (board->isConnected())
            {
               board->disconnectWifi();
            }

            
            if (board->connectWifi(wifiConfig.ssid, wifiConfig.password, WIFI_RETRY_TIME) == false)  // blocks
            {
               board->startAccessPoint(accessPointConfig.ssid, accessPointConfig.password);
            }
            break;
         }
         
         case ONLINE:
         {
            // TODO:
         }
      }
   }
   
   updateStatusLed();
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
   const String& host,
   const String& clientId,
   const String& clientPassword)
{
   serverConfig.host = host;
   serverConfig.clientId = clientId;
   serverConfig.clientPassword = clientPassword;
}

void Connection::setStatusLed(
   Led* statusLed)
{
   Connection::statusLed = statusLed;

   updateStatusLed();
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
            // TODO:
            break;
         }
      }
   }
}
