#include "UdpAdapter.hpp"

#include "Messaging.h"

void UdpAdapter::setup()
{
   printf(
      "UdpAdapter::setup: UDP Adapter [%s] is listening on port %d.\n",
      getId().c_str(),
      port);

   server.begin(port);
}

bool UdpAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      server.write(serializedMessage.c_str());
      server.endPacket();

      isSuccess = true;
   }

   return (isSuccess);
}

MessagePtr UdpAdapter::getRemoteMessage()
{
   static const int MAX_PACKET_SIZE = 255;

   MessagePtr message = 0;

   char buffer[MAX_PACKET_SIZE];
   String serializedMessage = "";

   int packetSize = server.parsePacket();

   // TODO: Handle multi-packet message.
   if (packetSize > 0)
   {
      // Read the packet.
      int length = server.read(buffer, 255);

      if (length > 0)
      {
         //  Null terminate and make into a String.
         buffer[length] = 0;
         serializedMessage = String(buffer);

         // Create a new message.
         message = Messaging::newMessage();

         if (message)
         {
            // Parse the message from the message string.
            if (protocol->parse(serializedMessage, message) == false)
            {
               // Parse failed.  Set the message free.
               message->setFree();
               message = 0;
            }
         }
      }
      else
      {
         // Ignore empty packets.
      }
   }

   return (message);
}
