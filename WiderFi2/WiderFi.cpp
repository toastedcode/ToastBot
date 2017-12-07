#include "Board.h"
#include "Logger.h"
#include "Properties.h"
#include "ToastBot.h"
#include "WiderFi.h"

//#include "lwip/ip.h"
//#include "lwip/netif.h"

extern "C" void ip_napt_enable(unsigned long addr, int enable);
extern "C" void ip_portmap_add(unsigned char proto, unsigned long maddr, unsigned int mport, unsigned long daddr, unsigned int dport);

#define IP_PROTO_TCP 6
#define IP_PROTO_UDP 17

void WiderFi::setup()
{
   WiFi.mode(WIFI_AP_STA);

   startAccessPoint(getUniqueId(), "");

   Timer* timer = Timer::newTimer("connectTimer", 20000, Timer::PERIODIC, this);
   timer->start();
}

void WiderFi::handleMessage(
      MessagePtr message)
{
   Logger::logDebug("WiderFi::handleMessage: %s", message->getMessageId().c_str());

   //  ping
   if (message->getMessageId() == "ping")
   {
      Message* reply = Messaging::newMessage();
      
      if (reply)
      {
         reply->setMessageId("pong");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("deviceId", ToastBot::getId());

         if (WifiBoard::getBoard())
         {
           unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
           WifiBoard::getBoard()->getMacAddress(mac);
           char macAddress[18];
           sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

           reply->set("macAddress", macAddress);
           reply->set("ipAddress", WifiBoard::getBoard()->getIpAddress());
         }

         Messaging::send(reply);

         message->setFree();
      }
   }
   else if (message->getMessageId() == "reset")
   {
      Logger::logDebug("Resetting ...");
      Board::getBoard()->reset();

      message->setFree();
   }
   else if (message->getMessageId() == "wifiConfig")
   {
      String ssid = message->getString("ssid");
      String password = message->getString("password");

      Logger::logDebug("WiderFi::handleMessage: wifiConfig(%s, %s)", ssid.c_str(), password.c_str());

      ToastBot::setProperty("ssid", ssid);
      ToastBot::setProperty("password", password);

      Connection::setWifiConfig(ssid, password);
   }
   else
   {
      Component::handleMessage(message);
   }
}

void WiderFi::timeout(
      Timer* timer)
{
   if (!isConnected())
   {
      //WifiConfig wifiConfig = Connection::getWifiConfig();

      if (wifiConfig.ssid != "")
      {
         connectWifi(wifiConfig.ssid, wifiConfig.password, 15);
      }
      else
      {
         Logger::logDebug("WiderFi::timeout: Checking for nearby Wider-Fi nodes.");
        
         // Scan for WiderFi hotspots.
         int apCount = WiFi.scanNetworks();
         
         for (int i = 0; i < apCount; i++)
         {
            if (WiFi.SSID(i).startsWith("WiderFi"))
            {
               if (connectWifi(WiFi.SSID(i), "", 15) == true)
               {
                  // Stop once we made a connection.
                  break;
               }
            }
         }
      }
   }
}

bool WiderFi::connectWifi(
   const String& ssid,
   const String& password,
   const int& connectionTimeout)
{
   Logger::logDebug(F("Connecting to Wifi network %s ..."), ssid.c_str());

   WiFi.begin(ssid.c_str(), password.c_str());

   int secondsToConnect = 0;

   // Wait until the connection is made, or until we timeout.
   while ((isConnected() == false) &&
          (secondsToConnect < connectionTimeout))
   {
      delay(1000);
      secondsToConnect++;
      Logger::logDebug(F("trying ..."));
   }

   if (isConnected())
   {
      Logger::logDebug(F("Success!  Connected at %s"), WiFi.localIP().toString().c_str());

      //ip_portmap_add(IP_PROTO_TCP, WiFi.localIP(), 8080, IPAddress(192,168,4,3), 80);
      //ip_portmap_add(IP_PROTO_UDP, WiFi.localIP(), 12345, IPAddress(192,168,4,2), 12345);
   }
   else
   {
      Logger::logDebug(F("Failure!  Could not connect to %s"), ssid.c_str());
   }

   return (isConnected());
}

bool WiderFi::startAccessPoint(
   const String& ssid,
   const String& password)
{
   Logger::logDebug(F("Creating wireless network %s."), ssid.c_str());

   WiFi.softAP(ssid.c_str(), password.c_str());

   //ip_napt_enable(IPAddress(192,168,4,1), 1);

   return (WiFi.status() == WL_CONNECTED);
}

bool WiderFi::isConnected() const
{
   return (WiFi.status() == WL_CONNECTED);  
}

String WiderFi::getUniqueId() const
{
   static const String PREFIX = "WiderFi";

   String uniqueId = "";
   char buffer[32];

  // Retrieve the MAC address of the board.
  unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
  WiFi.macAddress(mac);
  
  // Make an id out of the MAC address.
  sprintf(buffer,
          "%s_%02X%02X%02X%02X%02X%02X",
          PREFIX.c_str(),
          mac[0],
          mac[1],
          mac[2],
          mac[3],
          mac[4],
          mac[5]);

   return (String(buffer));          
}
