#include "IpServerAdapter.hpp"
#include "Logger.h"
#include "Messaging.h"

void IpServerAdapter::setup()
{
   Logger::logDebug(
      "IpServerAdapter::setup: IP Server Adapter [%s] is listening on port %d.",
      getId().c_str(),
      port);

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

         isSuccess = true;
      }
      else
      {
         Logger::logDebug(
            "IpServerAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.",
            message->getMessageId().c_str());
      }
   }

   return (isSuccess);
}

MessagePtr IpServerAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   String serializedMessage = "";

   if (!client.connected())
   {
      client = server.available();
   }

   static bool isConnected = false;
   bool wasConnected = isConnected;
   isConnected = client.connected();
   if (!wasConnected && isConnected)
   {
      Logger::logDebug("IpServerAdapter::getRemoteMessage: IP Server Adapter [%s] connected.", getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug("IpServerAdapter::getRemoteMessage: IP Server Adapter [%s] disconnected.", getId().c_str());
   }

   if ((client) && client.available())
   {
      serializedMessage = client.readStringUntil('\r');

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
