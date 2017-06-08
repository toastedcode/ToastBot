#include "Board.h"
#include "Logger.h"
#include "Properties.h"
#include "Robox.h"
#include "ToastBot.h"

void Robox::handleMessage(
      MessagePtr message)
{
   Logger::logDebug("Robox::handleMessage: %s", message->getMessageId().c_str());

   //  ping
   if (message->getMessageId() == "ping")
   {
      Message* reply = Messaging::newMessage();
      
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
   else if (message->getMessageId() == "reset")
   {
      Logger::logDebug("Resetting ...");
      Board::getBoard()->reset();
   }
   else if (message->getMessageId() == "property")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      Properties properties("/toastbot.properties");

      if (propertyName == "")
      {
         Logger::logDebug("Properties: \n%s", properties.toString().c_str());
      }
      else
      {
         if (propertyValue != "")
         {
            properties.set(propertyName, propertyValue);
            Logger::logDebug("Updated properties: %s = %s", propertyName.c_str(), propertyValue.c_str());
         }
         else
         {
            properties.remove(propertyName);
            Logger::logDebug("Removed property: %s", propertyName.c_str());
         }

         properties.save();
      }
   }
   else if (message->getMessageId() == "digitalWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("digitalWrite(%d, %d)", pin, value);
      Board::getBoard()->digitalWrite(pin, value);
   }
   else if (message->getMessageId() == "analogWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("analogWrite(%d, %d)", pin, value);
      Board::getBoard()->analogWrite(pin, value);
   }
   else if (message->getMessageId() == "bridge")
   {
      String name = message->getString("name");
      String host = message->getString("host");
      int port = message->getInt("port");
      
      Logger::logDebug("bridge(%s, %s:%d)", name.c_str(), host.c_str(), port);
      
      ToastBot::add(new TcpClientAdapter(name, new JsonProtocol(), host, port));
   }
   else
   {
      Component::handleMessage(message);
   }
}


String Robox::getUniqueId()
{
   static const String PREFIX = "ROBOX";

   String uniqueId = "";
   char buffer[32];

   WifiBoard* board = WifiBoard::getBoard();

   if (board)
   {
      // Retrieve the MAC address of the board.
      unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
      board->getMacAddress(mac);

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
   }
   else
   {
#ifdef ARDUINO
      // Seed the random number generator.
      randomSeed(analogRead(0));

      // Make an id out of some random numbers.
      sprintf(buffer,
              "%s_%02X%02X%02X%02X%02X%02X",
              PREFIX.c_str(),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255));
#endif
   }

   return (String(buffer));
}
