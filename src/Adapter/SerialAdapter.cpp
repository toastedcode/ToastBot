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

   String serializedMessage = Serial.readString();

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

   return (message);
}
