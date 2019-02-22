#include "Address.hpp"
#include "Connection/Connection.hpp"
#include "Messaging/JsonProtocol.hpp"
#include "Logger/RemoteLogger.hpp"
#include "ComponentFactory.hpp"
#include "Messaging.hpp"
#include "ToastBot.hpp"
#include "ToastBotMessageHandler.hpp"
#include "WifiBoard.hpp"

ToastBotMessageHandler::ToastBotMessageHandler() :
   Component("ToastBot")
{
}

ToastBotMessageHandler::~ToastBotMessageHandler()
{
}

void ToastBotMessageHandler::handleMessage(
   MessagePtr message)
{
   //  ping
   if (message->getMessageId() == "reset")
   {
      Logger::logDebug(F("ToastBot::handleMessage: reset()"));
      Board::getBoard()->reset();
   }
   else if (message->getMessageId() == "setProperty")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      Logger::logDebug(F("ToastBot::handleMessage: setProperty(%s, %s)"), propertyName.c_str(), propertyValue.c_str());

      Properties& properties = ToastBot::getProperties();

      if (propertyName != "")
      {
         if (propertyValue != "")
         {
            properties.set(propertyName, propertyValue);
            Logger::logDebug("ToastBot::handleMessage: Updated properties: %s = %s", propertyName.c_str(), propertyValue.c_str());
         }
         else
         {
            properties.erase(propertyName);
            Logger::logDebug("ToastBot::handleMessage: Removed property: %s", propertyName.c_str());
         }

         properties.save();
      }
   }
   else if (message->getMessageId() == "digitalRead")
   {
      int pin = message->getInt("pin");

      Logger::logDebug("ToastBot::handleMessage: digitalRead(%d)", pin);

      int value = Board::getBoard()->digitalRead(pin);

      Message* reply = Messaging::newMessage();

      if (reply)
      {
         reply->setMessageId("pinReading");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("pin", pin);
         reply->set("value", value);

         Messaging::send(reply);
      }
   }
   else if (message->getMessageId() == "digitalWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("ToastBot::handleMessage: digitalWrite(%d, %d)", pin, value);
      Board::getBoard()->digitalWrite(pin, value);
   }
   else if (message->getMessageId() == "analogRead")
   {
      int pin = message->getInt("pin");

      Logger::logDebug("ToastBot::handleMessage: analogRead(%d)", pin);

      int value = Board::getBoard()->analogRead(pin);

      Message* reply = Messaging::newMessage();

      if (reply)
      {
         reply->setMessageId("pinReading");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("pin", pin);
         reply->set("value", value);

         Messaging::send(reply);
      }
   }
   else if (message->getMessageId() == "analogWrite")
   {
      int pin = message->getInt("pin");
      int value = message->getInt("value");

      Logger::logDebug("ToastBot::handleMessage: analogWrite(%d, %d)", pin, value);
      Board::getBoard()->analogWrite(pin, value);
   }
   else if (message->getMessageId() == "create")
   {
      String componentDescription = message->getString("description");

      Logger::logDebug("ToastBot::handleMessage: create(%s)", componentDescription.c_str());

      JsonProtocol protocol;

      if (protocol.parse(componentDescription, message))
      {
        Logger::logDebug(F("ToastBot::handleMessage: Creating %s component [%s]"),
                         message->getString("class").c_str(),
                         message->getString("id").c_str());

        Component* component = ComponentFactory::create(message->getString("class"), message);
        if (component)
        {
          ToastBot::addComponent(component);
        }
      }
   }
   else if (message->getMessageId() == "setLogLevel")
   {
      LogLevel logLevel = fromString(message->getString("logLevel"));  // defaults to SEVERE

      Logger::logDebug("ToastBot::handleMessage: setLogLevel(%s)", toString(logLevel).c_str());

      Logger::setLogLevel(logLevel);
   }
   else if (message->getMessageId() == "remoteLogging")
   {
      bool enable = message->getBool("enable");

      String target = message->getSource();

      Logger::logDebug("ToastBot::handleMessage: remoteLogging(%s, %s)", (enable ? "true" : "false"), target.c_str());

      if (enable)
      {
         Logger::setLogger(new RemoteLogger(target));
      }
      else
      {
         Logger::setLogger(new SerialLogger());
      }
   }
   else if (message->getMessageId() == "wifiConfig")
   {
      Properties& properties = ToastBot::getProperties();

      String ssid = message->getString("ssid");
      String password = message->getString("password");

      if (message->getBool("query"))
      {
         Logger::logDebug("ToastBot::handleMessage: wifiConfig(query)");
      }
      else
      {
         Logger::logDebug("ToastBot::handleMessage: wifiConfig(%s, %s)", ssid.c_str(), password.c_str());

         if (message->isSet("ssid"))
         {
            properties.set("wifi.ssid", message->getString("ssid"));
         }

         if (message->isSet("password"))
         {
            properties.set("wifi.password", message->getString("password"));
         }

         properties.save();
      }

      MessagePtr reply = Messaging::newMessage();

      if (reply)
      {
         reply->setMessageId("wifiConfig");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("ssid", properties.getString("wifi.ssid"));
         reply->set("password", properties.getString("wifi.password"));

         Messaging::send(reply);
      }
   }
   else if (message->getMessageId() == "serverConfig")
   {
      Properties& properties = ToastBot::getProperties();

      String host = message->getString("host");
      int port = message->getInt("port");
      String userId = message->getString("userId");
      String password = message->getString("password");
      String topic = message->getString("topic");

      if (message->getBool("query"))
      {
         Logger::logDebug("ToastBot::handleMessage: serverConfig(query)");
      }
      else
      {
         Logger::logDebug("ToastBot::handleMessage: serverConfig(%s, %d, %s, %s, %s)",
                          host.c_str(),
                          port,
                          userId.c_str(),
                          password.c_str(),
                          topic.c_str());

         if (message->isSet("host"))
         {
            properties.set("server.host", host);
         }

         if (message->isSet("port"))
         {
            properties.set("server.port", port);
         }

         if (message->isSet("userId"))
         {
            properties.set("server.userId", userId);
         }

         if (message->isSet("password"))
         {
            properties.set("server.password", password);
         }

         if (message->isSet("topic"))
         {
            properties.set("server.topic", topic);
         }

         properties.save();
      }

      MessagePtr reply = Messaging::newMessage();

      if (reply)
      {
         reply->setMessageId("serverConfig");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("host", properties.getString("server.host"));
         reply->set("port", properties.getInt("server.port"));
         reply->set("userId", properties.getString("server.userId"));
         reply->set("password", properties.getString("server.password"));
         reply->set("topic", properties.getString("server.topic"));

         Messaging::send(reply);
      }
   }
   else if (message->getMessageId() == "connectionMode")
   {
      ConnectionMode mode = parseConnectionMode(message->getString("mode"));

      Logger::logDebug("ToastBot::handleMessage: connectionMode(%s)", toString(mode).c_str());

      Connection::setMode(mode);
   }
   else if (message->getMessageId() == "subscribe")
   {
      Address sourceAddress(message->getSource());

      String source = sourceAddress.getTop();
      String topic = message->getString("toTopic");

      Logger::logDebug("ToastBot::handleMessage: subscribe(%s, %s)", source.c_str(), topic.c_str());

      Component* component = ToastBot::getComponent(source);
      if (component)
      {
        Messaging::subscribe(component, topic);
      }
   }
   else if (message->getMessageId() == "heap")
   {
      long freeHeap = Board::getBoard()->getFreeHeap();

      Logger::logDebug("ToastBot::handleMessage: heap = %u", freeHeap);

      Message* reply = Messaging::newMessage();

      if (reply)
      {
         reply->setMessageId("heap");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("freeHeap", freeHeap);

         Messaging::send(reply);
      }
   }
   else
   {
      Component::handleMessage(message);
   }

   Messaging::freeMessage(message);
}


MessagePtr ToastBotMessageHandler::pingReply(
   MessagePtr ping)
{
   MessagePtr reply = Component::pingReply(ping);

   if (reply)
   {
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

      reply->set("apiKey", ToastBot::getProperties().getString("apiKey"));
   }

   return (reply);
}
