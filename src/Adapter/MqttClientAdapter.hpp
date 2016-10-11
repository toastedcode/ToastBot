#pragma once

#include "Adapter.hpp"
#include "ESP8266WiFi.h"
#include "Logger.hpp"
#include "PubSubClient.h"
#include "PubSubListener.h"

class MqttClientAdapter : public Adapter, PubSubListener
{

public:

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

private:

   static const int RETRY_DELAY;

   bool connect();

   String host;

   int port;

   String clientId;

   String userId;

   String password;
   
   bool isConnected;
   
   WiFiClient client;

   PubSubClient* mqttClient;

   long retryTime;
};
