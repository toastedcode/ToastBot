#include "Board.hpp"
#include "Logger.hpp"
#include "Messaging.h"
#include "TcpClientAdapter.hpp"

TcpClientAdapter::TcpClientAdapter(
   const String& id,
   Protocol* protocol,
   const String& host,
   const int& port,
   const int& retryDelay) :
      Adapter(id, protocol),
      host(host),
      port(port),
      retryTime(0),
      retryDelay(retryDelay),
      readIndex(0)
{
}

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

MessagePtr TcpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   static const char LF = '\n';
   static const char CR = '\r';

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
         Logger::logWarning("TcpClientAdapter::getRemoteMessage: Buffer overflow.  Discarding bytes.");

         readIndex = 0;
      }
   }

   return (message);
}

bool TcpClientAdapter::connect()
{
   bool success = client.connect(host.c_str(), port);

   if (!success)
   {
      retryTime = Board::getBoard()->systemTime()+ retryDelay;

      Logger::logDebug("TcpClientAdapter::connect: TCP Client Adapter [%s] failed to connect.", getId().c_str());
   }
   else
   {
      Logger::logDebug("TcpClientAdapter::connect: TCP Client Adapter [%s] connected.", getId().c_str());

      retryTime = 0;
   }

   return (success);
}

bool TcpClientAdapter::disconnect()
{
   // TODO
   return (false);
}
