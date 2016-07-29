#include "WebSocketAdapter.hpp"
#include "Messaging.h"

JsonProtocol WebSocketAdapter::protocol;

void WebSocketAdapter::setup()
{
}

bool WebSocketAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   uint8_t num = 0;
   String serializedMessage = protocol.serialize(message);

   if (serializedMessage != "")
   {
      webSocketServer.sendData(serializedMessage);
      isSuccess = true;
   }

   return (isSuccess);
}

MessagePtr WebSocketAdapter::getRemoteMessage()
{
   WiFiClient client = server.available();

   if (client.connected() && webSocketServer.handshake(client))
   {
      String serializedMessage = webSocketServer.getData();

      if (serializedMessage.length() > 0)
      {
         MessagePtr message = Messaging::newMessage();

         if (message != 0)
         {
            if (protocol.parse(serializedMessage, message) == true)
            {
               message->setSource(getAddress());

               Messaging::send(message);
             }
         }
      }
   }
}
