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

const int MqttClientAdapter::RETRY_DELAY = 3000;  // milliseconds

MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol) : ClientAdapter(id, protocol),
                         port(0),
                         connectionDesired(false),
                         retryTime(0),
                         mqttClient(0)
{
   mqttClient = new PubSubClient(client);
}


MqttClientAdapter::MqttClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port,
   const String& clientId,
   const String& userId,
   const String& password) : ClientAdapter(id, protocol),
                             host(host),
                             port(port),
                             clientId(clientId),
                             userId(userId),
                             password(password),
                             connectionDesired(false),
                             retryTime(0)
{
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
   //
   // MQTT connection
   //

   // Check connection state.
   bool wasConnected = isConnected();

   // Connection retry.
   if (connectionDesired &&
       !isConnected() &&
       (retryTime != 0) &&
       (Board::getBoard()->systemTime() > retryTime))
   {
      connectMqttClient();
   }

   if (!wasConnected && isConnected())
   {
      Logger::logDebug(
         F("MqttClientAdapter::loop: MQTT Client Adapter [%s] connected."), getId().c_str());

      String topic = "/toastbot/to/" + clientId;
      mqttClient->subscribe(topic.c_str());
   }
   else if (wasConnected && !isConnected())
   {
      Logger::logDebug(
         F("MqttClientAdapter::loop: MQTT Client Adapter [%s] disconnected."), getId().c_str());
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

   if (isConnected())
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

   if (isConnected())
   {
      disconnect();
   }

   Logger::logDebug(
      F("MqttClientAdapter::connect: Connecting to MQTT broker %s:%d as client [%s]."),
      host.c_str(),
      port,
      clientId.c_str());

   connectionDesired = true;
   retryTime = 0;

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

   connectionDesired = false;
   retryTime = 0;

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
   if ((this->host != host) ||
       (this->port != port))
   {
      Logger::logDebug("MqttClientAdapter::setServer: host = %s, port = %d", host.c_str(), port);

      this->host = host;
      this->port = port;

      mqttClient->setServer(host.c_str(), port);

      // Reconnect.
      if (isConnected())
      {
         disconnect();
         connect();
      }
   }
}

void MqttClientAdapter::setClientId(
   const String& clientId)
{
   if (this->clientId != clientId)
   {
      Logger::logDebug("MqttClientAdapter::setClientId: clientId = %s", clientId.c_str());

      this->clientId = clientId;

      // Reconnect.
      if (isConnected())
      {
         disconnect();
         connect();
      }
   }
}

void MqttClientAdapter::setUser(
   const String& userId,
   const String& password)
{
   if ((this->userId != userId) ||
       (this->password != password))
   {
      Logger::logDebug("MqttClientAdapter::setUser: userId = %s", userId.c_str());

      this->userId = userId;
      this->password = password;

      // Reconnect.
      if (isConnected())
      {
         disconnect();
         connect();
      }
   }
}

bool MqttClientAdapter::connectMqttClient()
{
   bool success = false;

   if (mqttClient )
   {
      // Attempt to connect to broker.
      // Note: Use user ID and password if provided.
      if (userId.length() == 0)
      {
         Logger::logDebug("MqttClientAdapter::retryConnect: user = %s, password = %s, clientId = %s", userId.c_str(), password.c_str(), clientId.c_str());
         success = mqttClient->connect(clientId.c_str());
      }
      else
      {
         success = mqttClient->connect(clientId.c_str(), userId.c_str(), password.c_str());
      }

      // Check our success and set up a retry if needed.
      if (success)
      {
         retryTime = 0;
      }
      else
      {
         Logger::logDebug(
            F("MqttClientAdapter::loop: Connect attempt failed.  (Reply code = %d).  Retrying in %d seconds."),
            mqttClient->state(),
            (RETRY_DELAY / 1000));

         retryTime = Board::getBoard()->systemTime() + RETRY_DELAY;
      }
   }

   return  (success);
}
