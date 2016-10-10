#include "Adapter.hpp"
#include "Logger.hpp"
#include "ESP8266WiFi.h"
#include "../Messaging/JsonProtocol.hpp"
#include "WebSocketServer.h"

class WebSocketAdapter : public Adapter
{

public:

   WebSocketAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   int port;

   WiFiServer server;

   WiFiClient client;

   WebSocketServer webSocketServer;

   bool isConnected;

   bool isNegotiated;
};

inline WebSocketAdapter::WebSocketAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) : Adapter(id, protocol),
                      port(port),
                      server(port),
                      isConnected(false),
                      isNegotiated(false)
{
}
