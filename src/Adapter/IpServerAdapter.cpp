#include "IpServerAdapter.hpp"
#include "Messaging.h"

void IpServerAdapter::setup()
{
}

bool IpServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      WiFiClient client = server.available();

      if (client && client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());
         isSuccess = true;
      }
   }

   return (isSuccess);
}

MessagePtr IpServerAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   String serializedMessage = "";

   WiFiClient client = server.available();

   if (client)
   {
      while (client.available() > 0)
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
            message->setSource(getAddress());
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
