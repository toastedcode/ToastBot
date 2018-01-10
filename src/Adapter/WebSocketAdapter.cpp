// *****************************************************************************
// *****************************************************************************
// WebSocketAdapter.cpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Logger.hpp"
#include "Messaging.h"
#include "WebSocketAdapter.hpp"

void WebSocketAdapter::setup()
{
   Logger::logDebug(
      F("WebSocketAdapter::setup: Web Socket Adapter [%s] is listening on port %d.\n"),
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
      Logger::logDebug(
         F("WebSocketAdapter::loop: Web Socket Server Adapter [%s] connected.\n"),
         getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug(
         F("WebSocketAdapter::loop: Web Socket Server Adapter [%s] disconnected.\n"),
         getId().c_str());

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
         Logger::logDebug(
            F("WebSocketAdapter::loop: Web Socket Server Adapter [%s] negotiated.\n"),
            getId().c_str());
      }
      else
      {
         Logger::logDebug(
            F("WebSocketAdapter::loop: Web Socket Server Adapter [%s] failed negotiation.\n"),
            getId().c_str());
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
      Logger::logWarning(
         F("IpServerAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.\n"),
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
               Messaging::freeMessage(message);
               message = 0;
            }
         }
      }
   }

   return (message);
}
