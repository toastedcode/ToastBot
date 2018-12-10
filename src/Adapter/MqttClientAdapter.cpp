// *****************************************************************************
// *****************************************************************************
// MqttClientAdapter.cpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Board.hpp"
#include "Logger.hpp"
#include "MqttClientAdapter.hpp"
#include "Messaging.h"

MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol) :
      ClientAdapter(id, protocol),
      port(0)
{
   mqttClient = new PubSubClient(client);
   mqttClient->setListener(this);
}

MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port,
   const String& clientId,
   const String& userId,
   const String& password) :
      ClientAdapter(id, protocol),
      host(host),
      port(port),
      clientId(clientId),
      userId(userId),
      password(password)
{
   mqttClient = new PubSubClient(client);

   mqttClient->setServer(host.c_str(), port);
   mqttClient->setListener(this);
}

MqttClientAdapter::MqttClientAdapter(
   MessagePtr parameters) :
      ClientAdapter(parameters)
{
   mqttClient = new PubSubClient(client);
   mqttClient->setListener(this);

   host = parameters->getString("host");
   port = parameters->getInt("port");
   clientId = parameters->getString("clientId");
   userId = parameters->getString("userId");
   password = parameters->getString("password");

   mqttClient = new PubSubClient(client);

   mqttClient->setServer(host.c_str(), port);
   mqttClient->setListener(this);
}

MqttClientAdapter::~MqttClientAdapter()
{
   delete (mqttClient);
}

void MqttClientAdapter::setup()
{
   Adapter::setup();
}

void MqttClientAdapter::loop()
{
   checkConnection();

   // Allow the MQTT client to do its processing.
   mqttClient->loop();

   // Hand processing off to the parent class.
   Adapter::loop();
}

bool MqttClientAdapter::subscribe(
   const String& topic)
{
   return (mqttClient && mqttClient->subscribe(topic.c_str()));
}

bool MqttClientAdapter::subscribe(
   const String& topic,
   const unsigned int qos)
{
   return (mqttClient && mqttClient->subscribe(topic.c_str(), qos));
}

bool MqttClientAdapter::unsubscribe(
   const String& topic)
{
   return (mqttClient && mqttClient->unsubscribe(topic.c_str()));
}

bool MqttClientAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   if (isConnected() && (publishTopic != ""))
   {
      String serializedMessage = protocol->serialize(message);

      if (serializedMessage != "")
      {
         isSuccess = mqttClient->publish(publishTopic.c_str(), serializedMessage.c_str());
      }
   }

   if (!isSuccess)
   {
      Logger::logWarning(
         F("MqttClientAdapter::sendRemoteMessage: Failed to send message [%s] to remote host."),
         message->getMessageId().c_str());
   }

   return (isSuccess);
}

MessagePtr MqttClientAdapter::getRemoteMessage()
{
   // Nothing to do here.
   return (NULL);
}

void MqttClientAdapter::callback(
   char* topic,
   unsigned char* payload,
   unsigned int length)
{
   static char buffer[MQTT_MAX_PACKET_SIZE + 1];

   if (length < MQTT_MAX_PACKET_SIZE)
   {
      // Construct a String from the payload.
      memcpy(buffer, payload, length);
      buffer[length] = 0;
      String serializedMessage(buffer);

      if (serializedMessage.length() > 0)
      {
         // Create a new message.
         MessagePtr message = Messaging::newMessage();

         if (message)
         {
            // Parse the message from the message string.
            if (protocol->parse(serializedMessage, message) == true)
            {
               message->setSource(getId());
               Messaging::send(message);
            }
            else
            {
               // Parse failed.  Set the message free.
               Messaging::freeMessage(message);
               message = 0;
            }
         }
      }
   }
}

bool MqttClientAdapter::connect()
{
   bool success = false;

   if (isConnected())
   {
      disconnect();
   }

   Logger::logDebug(
      F("MqttClientAdapter::connect: Connecting to MQTT broker %s:%d as client [%s]."),
      host.c_str(),
      port,
      clientId.c_str());

   return (connectMqttClient());
}


bool MqttClientAdapter::disconnect()
{
   if (isConnected())
   {
      Logger::logDebug(
         F("MqttClientAdapter::disconnect: Disconnecting from MQTT broker %s:%d."),
         host.c_str(),
         port);

      mqttClient->disconnect();
   }

   return (true);
}

bool MqttClientAdapter::isConnected()
{
   return (mqttClient && mqttClient->connected());
}


void MqttClientAdapter::setServer(
   const String& host,
   const int& port)
{
   this->host = host;
   this->port = port;

   mqttClient->setServer(this->host.c_str(), this->port);
}

void MqttClientAdapter::setClientId(
   const String& clientId)
{
   this->clientId = clientId;
}

void MqttClientAdapter::setUser(
   const String& userId,
   const String& password)
{
   this->userId = userId;
   this->password = password;
}

void MqttClientAdapter::setTopic(
   const String& topic)
{
   this->subscribeTopic = topic + "/to";
   this->publishTopic = topic + "/from";
}

void MqttClientAdapter::setAutoReconnect(
   const bool& autoReconnect)
{
   this->autoReconnect = autoReconnect;
}

bool MqttClientAdapter::connectMqttClient()
{
   bool success = false;

   if (mqttClient && (clientId != ""))
   {
      // Attempt to connect to broker.
      // Note: Use user ID and password if provided.
      if (userId.length() == 0)
      {
         success = mqttClient->connect(clientId.c_str());
      }
      else
      {
         success = mqttClient->connect(clientId.c_str(), userId.c_str(), password.c_str());
      }

      if (success)
      {
         Logger::logDebug(
            F("MqttClientAdapter::connectMqttClient: Successfully connected MQTT adapter [%s] to host [%s:%d]"),
            getId().c_str(),
            host.c_str(),
            port);

         if (subscribeTopic != "")
         {
            Logger::logDebug(
               F("MqttClientAdapter::connectMqttClient: Subscribing MQTT adapter [%s] to topic [%s]"),
               getId().c_str(),
               subscribeTopic.c_str());

            mqttClient->subscribe(subscribeTopic.c_str());
         }
         else
         {
            Logger::logWarning(
               F("MqttClientAdapter::connectMqttClient: No subscribe topic specified for MQTT adapter [%s]."),
               getId().c_str());
         }
      }
      else
      {
         Logger::logDebug(
            F("MqttClientAdapter::connectMqttClient: Connect attempt failed.  (Reply code = %d)."),
            mqttClient->state());
      }
   }

   return  (success);
}

void MqttClientAdapter::checkConnection()
{
   static bool wasConnected = false;

   bool isClientConnected = isConnected();

   if (wasConnected && !isClientConnected)
   {
      Logger::logDebug(
         F("MqttClientAdapter::checkConnection: MQTT adapter [%s] disconnected from host [%s:%d]."),
         getId().c_str(),
         host.c_str(),
         port);

      if (autoReconnect)
      {
         connectMqttClient();
      }
   }

   wasConnected = isClientConnected;
}
