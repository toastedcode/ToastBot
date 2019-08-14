// *****************************************************************************
// *****************************************************************************
// MqttClientAdapter.hpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "ClientAdapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWifi.hpp"
#include "PubSubClient.h"
#include "PubSubListener.h"

class MqttClientAdapter : public ClientAdapter, PubSubListener
{

public:

   MqttClientAdapter(
      const String& id,
      Protocol* protocol);

   MqttClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& host,
      const int& port,
      const String& clientId,
      const String& userId,
      const String& password);

   MqttClientAdapter(
      MessagePtr parameters);

   virtual ~MqttClientAdapter();

   virtual void setup();

   virtual void loop();

   bool subscribe(
      const String& topic);

   bool subscribe(
      const String& topic,
      const unsigned int qos);

   bool unsubscribe(
      const String& topic);

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   virtual void callback(
      char* topic,
      unsigned char* payload,
      unsigned int length);

   virtual bool connect();

   virtual bool disconnect();

   virtual bool isConnected();

   void setServer(
      const String& host,
      const int& port);

   void setClientId(
      const String& clientId);

   void setUser(
      const String& userId,
      const String& password);

   void setTopic(
      const String& topic);

   void setAutoReconnect(
      const bool& autoReconnect);

private:

   bool connectMqttClient();

   void checkConnection();

   String host;

   int port;

   String clientId;

   String userId;

   String password;
   
   String publishTopic;

   String subscribeTopic;
   
   bool autoReconnect;

   WiFiClient client;

   PubSubClient* mqttClient;
};

REGISTER(MqttClientAdapter, MqttClientAdapter)
