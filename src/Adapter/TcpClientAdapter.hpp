#include "Adapter.hpp"
#include "ESP8266WiFi.h"

class TcpClientAdapter : public Adapter
{

public:

   TcpClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& host,
      const int& port);

   virtual void setup();

   virtual void loop();

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

protected:

   bool connect();

   bool disconnect();

   static const int RETRY_DELAY;

   WiFiClient client;

   String host;

   int port;

   int retryTime;
};

inline TcpClientAdapter::TcpClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port) : Adapter(id, protocol),
                      host(host),
                      port(port),
                      retryTime(0)
{
}
