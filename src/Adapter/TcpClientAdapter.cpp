#include "Logger.hpp"
#include "Messaging.h"
#include "TcpClientAdapter.hpp"

const int TcpClientAdapter::RETRY_DELAY = 5000;  // 5 seconds

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
      if (client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());
         isSuccess = true;
      }
   }

   return (isSuccess);
}

MessagePtr TcpClientAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   static const int BUFFER_SIZE = 256;

   static const char LF = '\n';
   static const char CR = '\r';

   static char buffer[BUFFER_SIZE];

   static int readIndex = 0;

   if (client && client.available())
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
      retryTime = millis() + RETRY_DELAY;
   }
   else
   {
      retryTime = 0;
   }

   return (success);
}

bool TcpClientAdapter::disconnect()
{
   // TODO
   return (false);
}
