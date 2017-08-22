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
#include "ESP8266WiFi.h"
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

   ~MqttClientAdapter();

   virtual void setup();

   virtual void loop();

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

private:

   bool retryConnect();

   bool connectMqttClient();

   static const int RETRY_DELAY;

   String host;

   int port;

   String clientId;

   String userId;

   String password;
   
   bool connectionDesired;
   
   WiFiClient client;

   PubSubClient* mqttClient;

   long retryTime;
};
