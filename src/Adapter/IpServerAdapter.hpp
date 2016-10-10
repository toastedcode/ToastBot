#include "Adapter.hpp"
#include "Logger.hpp"
#include "ESP8266WiFi.h"

class IpServerAdapter : public Adapter
{

public:

   IpServerAdapter(
      const String& id,
      Protocol* protocol,
      const int& port);

   virtual void setup();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   int port;

   WiFiServer server;

   WiFiClient client;
};

inline IpServerAdapter::IpServerAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) : Adapter(id, protocol),
                      port(port),
                      server(port)
{
}
