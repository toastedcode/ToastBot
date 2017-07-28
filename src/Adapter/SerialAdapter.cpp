#include "Logger.hpp"
#include "Messaging.hpp"
#include "SerialAdapter.hpp"

SerialAdapter::SerialAdapter(
   const String& id,
   Protocol* protocol) :
   Adapter(id, protocol)
{
}

bool SerialAdapter::sendRemoteMessage(
   MessagePtr message)
{
   Serial.print(protocol->serialize(message));

   return (true);
}

MessagePtr SerialAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   static const int BUFFER_SIZE = 256;

   static const char LF = '\n';
   static const char CR = '\r';

   static char buffer[BUFFER_SIZE];

   static int readIndex = 0;

   while (Serial.available())
   {
      if (readIndex < BUFFER_SIZE)
      {
         char c = Serial.read();

         if (c == CR)
         {
            // Ignore.
         }
         else if (c == LF)
         {
            // Create the message string.
            buffer[readIndex] = 0;
            String serializedMessage = String(buffer);

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
         Logger::logWarning(F("SerialAdapter::getRemoteMessage: Buffer overflow.  Discarding bytes."));

         readIndex = 0;
      }
   }

   return (message);
}
