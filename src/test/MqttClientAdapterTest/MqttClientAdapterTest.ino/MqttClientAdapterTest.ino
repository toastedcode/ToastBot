#include <Board.h>
#include <ToastBot.h>
#include "Adapter\MqttClientAdapter.hpp"

static const int MESSAGE_POOL_SIZE = 5;
static const int TIMER_POOL_SIZE = 1;

static const String HOST = "test.mosquitto.org";
static const int PORT = 1883;
static const String CLIENT_ID = "MqttTest_";
static const String USER_ID = "";
static const String PASSWORD = "";
static const String MQTT_TOPIC = "robox/mqttTest";

class MqttTest : public Component, TimerListener
{
   
public:

   MqttTest() :
     Component("MqttTest"),
     adapter(0),
     iteration(0)
  {
  }

   void setup()
   {
      Timer* timer = Timer::newTimer("sendTimer", 1000, Timer::PERIODIC, this);
      timer->start();

      adapter = new MqttClientAdapter("mqtt_send", new JsonProtocol());
      adapter->setServer(HOST, PORT);
      adapter->setClientId(CLIENT_ID + "_01");
      adapter->setUser(USER_ID, PASSWORD);
      adapter->setTopic(MQTT_TOPIC);
      adapter->setup();

      adapter->connect();

      Messaging::registerHandler(adapter);

      Messaging::registerHandler(this);
   }

   void loop()
   {
      adapter->loop();
   }
   
   String getId() const
   {
      return ("MqttTest");
   }
   
   bool queueMessage(
      MessagePtr message)
   {
      Serial.printf("MqttTest::queueMessage: Got message [%s}, iteration %d", message->getMessageId().c_str(), message->getInt("iteration"));

      Messaging::freeMessage(message);
   }
      
   void timeout(
      Timer* timer)
   {
      iteration++;
      
      if (iteration < 30)
      {
         Message*  message = Messaging::newMessage();
         if (message)
         {
            message->setMessageId("ping");
            message->setDestination(getId());
            
            adapter->sendRemoteMessage(message);
            Messaging::freeMessage(message);
         }
      }
      else
      {
         timer->stop();
      }
   }

private:   

   MqttClientAdapter* adapter;

   int iteration;
};

MqttTest mqttTest;

void setup()
{
   Serial.begin(9600);
   Logger::setLogger(new SerialLogger());
   Logger::setLogLevel(DEBUG_FINEST);

   Logger::logDebug("\n\n********** Mqtt Cllient Adapter Test **********\n\n");

   Board::setBoard(new Esp8266Board());
   WifiBoard::getBoard()->connectWifi("REDACTED", "REDACTED");
   
   Messaging::setup(MESSAGE_POOL_SIZE);

   Timer::setup(TIMER_POOL_SIZE);
   
   mqttTest.setup();
}

void loop()
{
   Timer::loop();
   
   mqttTest.loop();

   // This is the key.  If we don't service the MQTT loop(), we get disconnected.
   delay(1000);
}
