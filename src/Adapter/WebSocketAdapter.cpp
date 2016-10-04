#include "WebSocketAdapter.hpp"
#include "Messaging.h"

void WebSocketAdapter::setup()
{
}

bool WebSocketAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   uint8_t num = 0;
   String serializedMessage = protocol->serialize(message);

   if (serializedMessage != "")
   {
      webSocketServer.sendData(serializedMessage);
      isSuccess = true;
   }

   return (isSuccess);
}

MessagePtr WebSocketAdapter::getRemoteMessage()
{
   MessagePtr message = 0;

   WiFiClient client = server.available();

   if (client.connected() && webSocketServer.handshake(client))
   {
      String serializedMessage = webSocketServer.getData();

      if (serializedMessage.length() > 0)
      {
         // Create a new message.
         message = Messaging::newMessage();

         // Parse the message from the message string.
         if (protocol->parse(serializedMessage, message) == true)
         {
            // Parse was successful.
            message->setSource(getId());
         }
         else
         {
            // Parse failed.  Set the message free.
            message->setFree();
            message = 0;
         }
      }
   }

   return (message);
}
