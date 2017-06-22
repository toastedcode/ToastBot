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

      message->setFree();
   }
   else if (message->getMessageId() == "property")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      Properties properties("/toastbot.properties");

      if (propertyName == "")
      {
         Logger::logDebug("Robox::handleMessage: Properties: \n%s", properties.toString().c_str());
      }
      else
      {
         if (propertyValue != "")
         {
            properties.set(propertyName, propertyValue);
            Logger::logDebug("Robox::handleMessage: Updated properties: %s = %s", propertyName.c_str(), propertyValue.c_str());
         }
         else
         {
            properties.remove(propertyName);
            Logger::logDebug("Robox::handleMessage: Removed property: %s", propertyName.c_str());
         }

         properties.save();
      }

      message->setFree();
   }
   else if (message->getMessageId() == "digitalWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("Robox::handleMessage: digitalWrite(%d, %d)", pin, value);
      Board::getBoard()->digitalWrite(pin, value);

      message->setFree();
   }
   else if (message->getMessageId() == "analogWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("Robox::handleMessage: analogWrite(%d, %d)", pin, value);
      Board::getBoard()->analogWrite(pin, value);

      message->setFree();
   }
   else if (message->getMessageId() == "bridge")
   {
      String name = message->getString("name");
      String host = message->getString("host");
      int port = message->getInt("port");
      
      Logger::logDebug("Robox::handleMessage: bridge(%s, %s:%d)", name.c_str(), host.c_str(), port);
      
      ToastBot::add(new TcpClientAdapter(name, new JsonProtocol(), host, port));

      message->setFree();
   }
   else if (message->getMessageId() == "create")
   {
      String classId = message->getString("class");
      String id = message->getString("id");

      Logger::logDebug("Robox::handleMessage: create(%s, %s)", classId.c_str(), id.c_str());

      Component* component = ComponentFactory::create(classId, message);

      if (component)
      {
         ToastBot::add(component);
      }

      message->setFree();
   }
   else if (message->getMessageId() == "setLogger")
   {
      String adapterId = message->getString("adapter");

      Logger::logDebug("Robox::handleMessage: setLogger(%s)", adapterId.c_str());

      if (ToastBot::get(adapterId))
      {
        Logger::setLogger(new RemoteLogger(adapterId));
      }
      else
      {
        Logger::logDebug("Robox::handleMessage: No adapter [%s] available.", adapterId.c_str());
      }

      message->setFree();
   }
   else if (message->getMessageId() == "setLogLevel")
   {
      LogLevel logLevel = fromString(message->getString("logLevel"));  // defaults to SEVERE

      Logger::logDebug("Robox::handleMessage: setLogLevel(%s)", toString(logLevel).c_str());

      Logger::setLogLevel(logLevel);

      message->setFree();
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
