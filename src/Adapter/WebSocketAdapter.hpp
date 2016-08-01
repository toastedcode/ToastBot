#include "Adapter.hpp"
#include "Logger.h"
#include "ESP8266WiFi.h"
#include "../Messaging/JsonProtocol.hpp"
#include "WebSocketServer.h"

class WebSocketAdapter : public Adapter
{

public:

   WebSocketAdapter(
      const String& id,
      Protocol* protocol);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   WiFiServer server;

   WebSocketServer webSocketServer;

};

inline WebSocketAdapter::WebSocketAdapter(
   const String& id,
   Protocol* protocol) : Adapter(id, protocol),
                         server(81)
{

}
