#include "Logger.hpp"
#include "Messaging.h"
#include "TcpServerAdapter.hpp"

const int TcpServerAdapter::BUFFER_SIZE;

TcpServerAdapter::TcpServerAdapter(
   const String& id,
   Protocol* protocol,
   const int& port) :
      Adapter(id, protocol),
      port(port),
      server(port),
      isConnected(false)
{
}

void TcpServerAdapter::setup()
{
   Logger::logDebug(
      "TcpServerAdapter::setup: TCP Server Adapter [%s] is listening on port %d.",
      getId().c_str(),
      port);

   server.begin();
}

bool TcpServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      serializedMessage += "\n";

      if (client && client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());

         isSuccess = true;
      }
      else
      {
         Logger::logWarning(
            "TcpServerAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.",
            message->getMessageId().c_str());
      }
   }

   return (isSuccess);
}

MessagePtr TcpServerAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   static const char LF = '\n';
   static const char CR = '\r';

   // Check for a TCP connection.
   if (!client.connected())
   {
      client = server.available();
   }

   bool wasConnected = isConnected;
   isConnected = client.connected();

   if (!wasConnected && isConnected)
   {
      Logger::logDebug("TcpServerAdapter::getRemoteMessage: TCP Server Adapter [%s] connected.", getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug("TcpServerAdapter::getRemoteMessage: TCP Server Adapter [%s] disconnected.", getId().c_str());
   }

   while (client && client.available())
   {
      if (readIndex < BUFFER_SIZE)
      {
         char c = client.read();

         if ((c == CR) || (c == LF))
         {
            // Create the message string.
            buffer[readIndex] = 0;
            String serializedMessage = String(buffer);

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

            // Reset the read index.
            readIndex = 0;
         }
         else
         {
            // Store the character.
            buffer[readIndex] = c;
            readIndex++;
         }
      }
      else
      {
         Logger::logWarning("TcpServerAdapter::getRemoteMessage: Buffer overflow.  Discarding bytes.");

         readIndex = 0;
      }
   }

   return (message);
}
