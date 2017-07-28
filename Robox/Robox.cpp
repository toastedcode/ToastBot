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
   else if (message->getMessageId() == "properties")
   {
      Properties& properties = ToastBot::getProperties();
    
      Logger::logDebug("Robox::handleMessage: Properties:");
      properties.log();
   }
   else if (message->getMessageId() == "property")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      Properties& properties = ToastBot::getProperties();

      if (propertyName == "")
      {
         Logger::logDebug("Robox::handleMessage: Properties:");
         properties.log();
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
      
      ToastBot::addComponent(new TcpClientAdapter(name, new JsonProtocol(), host, port, 5000));

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
         ToastBot::addComponent(component);
      }

      message->setFree();
   }
   else if (message->getMessageId() == "setLogger")
   {
      String adapterId = message->getString("adapter");

      Logger::logDebug("Robox::handleMessage: setLogger(%s)", adapterId.c_str());

      if (ToastBot::getComponent(adapterId))
      {
        Logger::setLogLevel(DEBUG);
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
   else if (message->getMessageId() == "wifiConfig")
   {
      String ssid = message->getString("ssid");
      String password = message->getString("password");

      Logger::logDebug("Robox::handleMessage: wifiConfig(%s, %s)", ssid.c_str(), password.c_str());

      ToastBot::setProperty("ssid", ssid);
      ToastBot::setProperty("password", password);

      Connection::setWifiConfig(ssid, password);
   }
   else if (message->getMessageId() == "mode")
   {
      ConnectionMode mode = parseConnectionMode(message->getString("mode"));

      Logger::logDebug("Robox::handleMessage: connectionMode(%s)", toString(mode).c_str());

      Connection::setMode(mode);
   }
   else if (message->getMessageId() == "subscribe")
   {
      String source = message->getSource();
      String topic = message->getString("toTopic");

      Logger::logDebug("Robox::handleMessage: subscribe(%s, %s)", source.c_str(), topic.c_str());

      Component* component = ToastBot::getComponent(source);
      if (component)
      {
        Messaging::subscribe(component, topic);
      }
   }   
   else if (message->getMessageId() == "health")
   {
      // Start the health monitor once we get a ping.
      HealthMonitor* healthMonitor = (HealthMonitor*)ToastBot::getComponent("healthMonitor");
      if (healthMonitor)
      {
         Logger::logDebug("Robox::handleMessage: Starting health monitor.");
         healthMonitor->start();
      } 
   }
   else if (message->getMessageId() == "heap")
   {
      Logger::logDebug("Robox::handleMessage: heap = %u", Board::getBoard()->getFreeHeap());
   }
   else
   {
      Component::handleMessage(message);
   }
}

