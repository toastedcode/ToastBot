#include "MqttClientAdapter.hpp"
#include "Messaging.h"

MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port,
   const String& user,
   const String& password) : IpClientAdapter(id, protocol, host, port),
                             user(user),
                             password(password),
                             isConnected(false)
{
   messageQueue = new StaticMessageQueue(10);
   mqttClient = new PubSubClient(client);
}

MqttClientAdapter::~MqttClientAdapter()
{
   delete (messageQueue);
   delete (mqttClient);
}

void MqttClientAdapter::setup()
{
   mqttClient->setServer(host, port);
   mqttClient->setCallback(this);
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
         isConnected = mqttClient->connect(user);

         if (isConnected == false)
         {
            retryTime = millis() + RETRY_DELAY;
         }
      }
   }

   if (!wasConnected && isConnected)
   {
      Logger::logDebug("MqttClientAdapter::loop: MQTT Client Adapter [%s] connected.", getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug("MqttClientAdapter::loop: MQTT Client Adapter [%s] disconnected.", getId().c_str());
   }

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
         mqttClient->publish(message->getString("topic"), serializedMessage);
         isSuccess = true;
      }
   }
   else
   {
      Logger::logDebug(
         "MqttClientAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.",
         message->getMessageId().c_str());
   }

   return (isSuccess);
}

MessagePtr MqttClientAdapter::getRemoteMessage()
{
   return (messageQueue->dequeue());
}

void MqttClientAdapter::callback(
   char* topic,
   unsigned char* payload,
   unsigned int length)
{
   Message* message = 0;

   String serializedMessage(payload, length);

   if (serializedMessage.length() > 0)
   {
      // Create a new message.
      message = Messaging::newMessage();

      if (message)
      {
         // Parse the message from the message string.
         if (protocol->parse(serializedMessage, message) == true)
         {
            // Parse was successful.
            message->set("topic", String(topic));

            messageQueue->enqueue(message);
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
