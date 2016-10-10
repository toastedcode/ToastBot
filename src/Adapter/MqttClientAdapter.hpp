#include "Adapter.hpp"
#include "Logger.hpp"
#include "PubSubClient.h"
#include "PubSubListener.h"
#include "StaticMessageQueue.h"

class MqttClientAdapter : public Adapter, PubSubListener
{

public:

   MqttClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& host,
      const int& port,
      const String& user,
      const String& password);

   virtual void setup();
   
   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   virtual void callback(
      char* topic,
      unsigned char* payload,
      unsigned int length);

protected:

   String user;

   String password;
   
   bool isConnected;
   
   PubSubClient* mqttClient;
   
   MessageQueue* messageQueue;
};