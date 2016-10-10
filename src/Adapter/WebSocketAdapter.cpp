#include "WebSocketAdapter.hpp"
#include "Messaging.h"

void WebSocketAdapter::setup()
{
   Logger::logDebug(
      "WebSocketAdapter::setup: Web Socket Adapter [%s] is listening on port %d.",
      getId().c_str(),
      port);

   server.begin();
}

void WebSocketAdapter::loop()
{
   //
   // Socket connection
   //

   // Check connection state.
   bool wasConnected = isConnected;
   isConnected = client.connected();

   // Attempt to connect.
   if (!isConnected)
   {
      client = server.available();
      isConnected = client.connected();
   }

   if (!wasConnected && isConnected)
   {
      Logger::logDebug("WebSocketAdapter::loop: Web Socket Server Adapter [%s] connected.", getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug("WebSocketAdapter::loop: Web Socket Server Adapter [%s] disconnected.", getId().c_str());

      isNegotiated = false;
   }

   //
   // Web Socket negotiation
   //

   bool wasNegotiated = isNegotiated;

   if (isConnected && !isNegotiated)
   {
      isNegotiated = webSocketServer.handshake(client);

      if (!wasNegotiated && isNegotiated)
      {
         Logger::logDebug("WebSocketAdapter::loop: Web Socket Server Adapter [%s] negotiated.", getId().c_str());
      }
      else
      {
         Logger::logDebug("WebSocketAdapter::loop: Web Socket Server Adapter [%s] failed negotiation.", getId().c_str());
      }
   }

   // Hand processing off to the parent class.
   Adapter::loop();
}

bool WebSocketAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   if (isConnected && isNegotiated)
   {
      String serializedMessage = protocol->serialize(message);

      if (serializedMessage != "")
      {
         webSocketServer.sendData(serializedMessage);
         isSuccess = true;
      }
   }
   else
   {
      Logger::logDebug(
         "IpServerAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.",
         message->getMessageId().c_str());
   }

   return (isSuccess);
}

MessagePtr WebSocketAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   if (isConnected && isNegotiated)
   {
      String serializedMessage = webSocketServer.getData();

      if (serializedMessage.length() > 0)
      {
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
   }

   return (message);
}
