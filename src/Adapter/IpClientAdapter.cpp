#include "IpClientAdapter.hpp"
#include "Messaging.h"

const int IpClientAdapter::RETRY_DELAY = 5000;  // 5 seconds

void IpClientAdapter::setup()
{
   connect();
}

void IpClientAdapter::loop()
{
   Adapter::loop();

   // Connection retry.
   if ((retryTime != 0) &&
       (millis() > retryTime))
   {
      connect();
   }
}

bool IpClientAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      if (client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());
         isSuccess = true;
      }
   }

   return (isSuccess);
}

MessagePtr IpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   String serializedMessage = "";

   while ((client.connected()) &&
          (client.available() > 0))
   {
      serializedMessage += client.read();
   }

   if (serializedMessage.length() > 0)
   {
      // Create a new message.
      message = Messaging::newMessage();

      // Parse the message from the message string.
      if (protocol->parse(serializedMessage, message) == true)
      {
         // Parse was successful.
         message->setSource(getId());
      }
      else
      {
         // Parse failed.  Set the message free.
         message->setFree();
         message = 0;
      }
   }

   return (message);
}

bool IpClientAdapter::connect()
{
   bool success = client.connect(host.c_str(), port);

   if (!success)
   {
      retryTime = millis() + RETRY_DELAY;
   }
   else
   {
      retryTime = 0;
   }

   return (success);
}

bool IpClientAdapter::disconnect()
{
   // TODO
   return (false);
}
