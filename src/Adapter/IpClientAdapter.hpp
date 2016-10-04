#include "Adapter.hpp"
#include "Logger.h"
#include "ESP8266WiFi.h"

class IpClientAdapter : public Adapter
{

public:

   IpClientAdapter(
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

inline IpClientAdapter::IpClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port) : Adapter(id, protocol),
                      host(host),
                      port(port),
                      retryTime(0)
{
}
