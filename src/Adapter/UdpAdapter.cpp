#include "Board.hpp"
#include "Messaging.h"
#include "UdpAdapter.hpp"
#include "StringUtils.hpp"

UdpAdapter::UdpAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) :
      Adapter(id, protocol),
      listenPort(port),
      sendPort(0)
{
}

UdpAdapter::UdpAdapter(
   const String& id,
   Protocol* protocol,
   const IPAddress& ipAddress,
   const int& port) :
      Adapter(id, protocol),
      listenPort(0),
      sendIpAddress(ipAddress),
      sendPort(port)
{
}

void UdpAdapter::setup()
{
   if (listenPort != 0)
   {
      printf(
         "UdpAdapter::setup: UDP Adapter [%s] is listening on port %d.\n",
         getId().c_str(),
         listenPort);

      server.begin(listenPort);
   }
}

void UdpAdapter::loop()
{
   MessagePtr message = NULL;

   //
   // Send
   //

   Component::loop();

   //
   // Receive
   //

   message = getRemoteMessage();
   while (message != NULL)
   {
      // Disable this.  We'll set the source id to the "reply adapter" in getRemoteMessage().
      //message->setSource(getId());

      MessageRouter::send(message);

      message = getRemoteMessage();
   }
}

bool UdpAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   IPAddress remoteIpAddress;
   int remotePort = 0;

   // Try to parse the remote IP address and port from the reply adapter id.
   parseReplyAdapterId(message->getDestination(), remoteIpAddress, remotePort);

   // If no remote address was specified, use the configured send address.
   if (remotePort == 0)
   {
      remoteIpAddress = sendIpAddress;
      remotePort = sendPort;
   }

   // If we have a valid destination ...
   if (remotePort != 0)
   {
      String serializedMessage = protocol->serialize(message);

      if (serializedMessage != "")
      {
         if (server.beginPacket(remoteIpAddress, remotePort) != 0)
         {
            printf("UdpAdapter::sendRemoteMessage Sending to %s:%d\n", remoteIpAddress.toString().c_str(), remotePort);

            server.write(serializedMessage.c_str(), serializedMessage.length());
            isSuccess = server.endPacket();
         }
      }
   }

   return (isSuccess);
}

MessagePtr UdpAdapter::getRemoteMessage()
{
   static const int MAX_PACKET_SIZE = 255;

   MessagePtr message = 0;

   // If this is not a send-only adapter ...
   if (listenPort != 0)
   {
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
               else
               {
                  // Create a "reply adapter" that we can use to reply to this message.
                  String replyAdapterId = getReplyAdapterId(server.remoteIP(), server.remotePort());
                  message->setSource(replyAdapterId);
               }
            }
         }
         else
         {
            // Ignore empty packets.
         }
      }
   }

   return (message);
}

String UdpAdapter::getReplyAdapterId(
   const IPAddress& ipAddress,
   const int& port) const
{
   static const String PREFIX = "UDP_REPLY_";

   // Create a unique id for this adapter.
   IPAddress nonConstIpAddress = ipAddress;
   String replyAdapterId = getId() + MESSAGE_HANDLER_ID_SEPARATOR + nonConstIpAddress.toString() + ":" + String(port);

   return (replyAdapterId);
}

void UdpAdapter::parseReplyAdapterId(
   const String& replyAdapterId,
   IPAddress& ipAddress,
   int& port)
{
   // Parses: udpadapter@10.4.41.179:8080 into 10.4.41.179 and 8080.

   // Find the "@".
   int pos = StringUtils::findFirstOf(replyAdapterId, MESSAGE_HANDLER_ID_SEPARATOR);

   if (pos != -1)
   {
      String destination = replyAdapterId.substring(pos + 1);

      // Find the ":";
      pos = StringUtils::findFirstOf(destination, ":");

      if (pos != -1)
      {
         String ipAddressStr = destination.substring(0, pos);
         String portStr = destination.substring(pos + 1);

         ipAddress.fromString(ipAddressStr.c_str());
         port = portStr.toInt();
      }
   }
}
