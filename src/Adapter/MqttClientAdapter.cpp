// *****************************************************************************
// *****************************************************************************
// MqttClientAdapter.cpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Logger.hpp"
#include "MqttClientAdapter.hpp"
#include "Messaging.h"

const int MqttClientAdapter::RETRY_DELAY = 3000;  // milliseconds

MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port,
   const String& clientId,
   const String& userId,
   const String& password) : Adapter(id, protocol),
                             host(host),
                             port(port),
                             clientId(clientId),
                             userId(userId),
                             password(password),
                             isConnected(false),
                             retryTime(0)
{
   mqttClient = new PubSubClient(client);
}

MqttClientAdapter::~MqttClientAdapter()
{
   delete (mqttClient);
}

void MqttClientAdapter::setup()
{
   mqttClient->setServer(host.c_str(), port);
   mqttClient->setListener(this);
   retryTime = millis();
}

void MqttClientAdapter::loop()
{
   //
   // MQTT connection
   //

   // Check connection state.
   bool wasConnected = isConnected;
   isConnected = mqttClient->connected();

   if (!isConnected)
   {
      // Connection retry.
      if ((retryTime != 0) &&
          (millis() > retryTime))
      {
         isConnected = connect();

         if (isConnected == false)
         {
            retryTime = millis() + RETRY_DELAY;

            Logger::logDebug(
               F("MqttClientAdapter::loop: Connect attempt failed.  Retrying in %d seconds.\n"),
               (RETRY_DELAY / 1000));
         }
      }
   }

   if (!wasConnected && isConnected)
   {
      Logger::logDebug(
         F("MqttClientAdapter::loop: MQTT Client Adapter [%s] connected.\n"), getId().c_str());

      String topic = "/toastbot/to/" + clientId;
      mqttClient->subscribe(topic.c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug(
         F("MqttClientAdapter::loop: MQTT Client Adapter [%s] disconnected.\n"), getId().c_str());
   }

   // Allow the MQTT client to do its processing.
   mqttClient->loop();

   // Hand processing off to the parent class.
   Adapter::loop();
}

bool MqttClientAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   if (isConnected)
   {
      String serializedMessage = protocol->serialize(message);

      if (serializedMessage != "")
      {
         String topic = "/toastbot/from/" + clientId;

         isSuccess = mqttClient->publish(topic.c_str(), serializedMessage.c_str());
      }
   }

   if (!isSuccess)
   {
      Logger::logWarning(
         F("MqttClientAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.\n"),
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
               message->setFree();
               message = 0;
            }
         }
      }
   }
}

bool MqttClientAdapter::connect()
{
   bool success = false;

   if (mqttClient)
   {
      Logger::logDebug(
         F("MqttClientAdapter::connect: Connecting to MQTT broker %s:%d as client [%s]."),
         host.c_str(),
         port,
         clientId.c_str(),
         userId.c_str());

      if (userId.length() > 0)
      {
         success = mqttClient->connect(clientId.c_str(), userId.c_str(), password.c_str());
      }
      else
      {
         success = mqttClient->connect(clientId.c_str());
      }
   }

   return (success);
}
