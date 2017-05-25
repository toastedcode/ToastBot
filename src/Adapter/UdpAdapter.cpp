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

bool UdpAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   IPAddress remoteIpAddress;
   int remotePort = 0;

   // Try to parse the remote IP address and port from the reply adapter id.
   parseSocketString(message->getDestination(), remoteIpAddress, remotePort);

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
                  // Address the message from the ipaddress:port from which we received the message..
                  String src = getSocketString(server.remoteIP(), server.remotePort());
                  message->setSource(src);
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

String UdpAdapter::getSocketString(
   const IPAddress& ipAddress,
   const int& port)
{
   // Create a unique id for this adapter.
   IPAddress nonConstIpAddress = ipAddress;
   String destination = nonConstIpAddress.toString() + ":" + String(port);

   return (destination);
}

void UdpAdapter::parseSocketString(
   const String& socketString,
   IPAddress& ipAddress,
   int& port)
{
   // Parses: 10.4.41.179:8080 into 10.4.41.179 and 8080.

   // Initialize the return value.
   port = 0;

   // Find the ":";
   int pos = StringUtils::findFirstOf(socketString, ":");

   if (pos != -1)
   {
      String ipAddressStr = socketString.substring(0, pos);
      String portStr = socketString.substring(pos + 1);

      ipAddress.fromString(ipAddressStr.c_str());
      port = portStr.toInt();
   }
}
