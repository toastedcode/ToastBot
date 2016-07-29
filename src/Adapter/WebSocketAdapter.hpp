#include "Adapter.hpp"
#include "Logger.h"
#include "ESP8266WiFi.h"
#include "../Messaging/JsonProtocol.hpp"
#include "WebSocketServer.h"

class WebSocketAdapter : public Adapter
{

public:

   WebSocketAdapter(String id);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   static JsonProtocol protocol;

   WiFiServer server;

   WebSocketServer webSocketServer;

};

inline WebSocketAdapter::WebSocketAdapter(String id) :
   Adapter(id),
   server(81)
{

}
