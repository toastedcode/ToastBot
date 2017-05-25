#include "Messaging.h"
#include "TcpClientAdapter.hpp"

const int TcpClientAdapter::RETRY_DELAY = 5000;  // 5 seconds

void TcpClientAdapter::setup()
{
   connect();
}

void TcpClientAdapter::loop()
{
   Adapter::loop();

   // Connection retry.
   if ((retryTime != 0) &&
       (millis() > retryTime))
   {
      connect();
   }
}

bool TcpClientAdapter::sendRemoteMessage(
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

MessagePtr TcpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   String serializedMessage = "";

   if ((client) && client.available())
   {
      serializedMessage = client.readStringUntil('\r');

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
   }

   return (message);
}

bool TcpClientAdapter::connect()
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

bool TcpClientAdapter::disconnect()
{
   // TODO
   return (false);
}
