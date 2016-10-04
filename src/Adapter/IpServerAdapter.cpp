#include "IpServerAdapter.hpp"
#include "Messaging.h"

void IpServerAdapter::setup()
{
   server.begin();
}

bool IpServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      if (client && client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());

         printf("Sent\n");

         isSuccess = true;
      }
      else
      {
         printf("Failed to send\n");
      }
   }

   return (isSuccess);
}

MessagePtr IpServerAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   String serializedMessage = "";

   if ((!client) || (!client.connected()))
   {
      client = server.available();
   }

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
