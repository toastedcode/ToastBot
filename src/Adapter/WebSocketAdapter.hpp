// *****************************************************************************
// *****************************************************************************
// WebSocketAdapter.hpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWifi.hpp"
#include "../Messaging/JsonProtocol.hpp"

// TODO: Make portable between ESP8266 and ESP32
#ifdef ESP8266
#include "WebSocketServer.h"
#endif

class WebSocketAdapter : public Adapter
{

public:

   WebSocketAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   WebSocketAdapter(
      MessagePtr parameters);

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   int port;

   WiFiServer server;

   WiFiClient client;

// TODO: Make portable between ESP8266 and ESP32
#ifdef ESP8266
   WebSocketServer webSocketServer;
#endif

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

REGISTER(WebSocketAdapter, WebSocketAdapter)
