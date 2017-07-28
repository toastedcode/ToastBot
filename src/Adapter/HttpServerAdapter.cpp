#include "../Messaging/JsonProtocol.hpp"
#include "../Messaging/RestfulProtocol.hpp"
#include "Logger.hpp"
#include "Messaging.h"
#include "HttpServerAdapter.hpp"

HttpServerAdapter::HttpServerAdapter(
   const String& id) :
      TcpServerAdapter(id, new RestfulProtocol(), 80)
{
}

bool HttpServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   bool isSuccess = false;

   static const char* HTTP_HEADER =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/json\r\n\r\n";

   static JsonProtocol jsonProtocol;

   String serializedMessage = jsonProtocol.serialize(message);

   if (serializedMessage != "")
   {
      String httpString = HTTP_HEADER;
      httpString += serializedMessage;
      serializedMessage += "\r\n";

      if (client && client.connected())
      {
         client.write(serializedMessage.c_str(), serializedMessage.length());

         isSuccess = true;
      }
      else
      {
         Logger::logWarning(
            "HttpServerAdapter::sendRemoteMessage: Failed to send message [%s] to remote host.",
            message->getMessageId().c_str());
      }
   }

   return (isSuccess);
}

MessagePtr HttpServerAdapter::getRemoteMessage()
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
      Logger::logDebug("HttpServerAdapter::getRemoteMessage: TCP Server Adapter [%s] connected.", getId().c_str());
   }
   else if (wasConnected && !isConnected)
   {
      Logger::logDebug("HttpServerAdapter::getRemoteMessage: TCP Server Adapter [%s] disconnected.", getId().c_str());
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

            serializedMessage = getMessageFromHttpRequest(serializedMessage);

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

                     // Send a 404 response.
                     String response =
                        "HTTP/1.0 404 NOT FOUND\r\n"
                        "Content-Type: text/html\r\n\r\n"
                        "<!DOCTYPE HTML>\r\n"
                        "<html>Could not process your request</html>\r\n";
                     client.write(response.c_str(), response.length());
                  }
                  else
                  {
                     // Send a 200 response.
                     String response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n\r\n"
                        "<!DOCTYPE HTML>\r\n"
                        "<html>Request processed</html>\r\n";
                     client.write(response.c_str(), response.length());
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
         Logger::logWarning("HttpServerAdapter::getRemoteMessage: Buffer overflow.  Discarding bytes.");

         readIndex = 0;
      }
   }

   return (message);
}

String HttpServerAdapter::getMessageFromHttpRequest(
   const String& httpRequestString)
{
   String messageString;

   if ((httpRequestString.length() > 0) &&
       (httpRequestString.indexOf("GET") != -1))
   {
      int startPos = httpRequestString.indexOf("GET") + 3;
      int endPos = httpRequestString.indexOf("HTTP/1.1");
      endPos = (endPos == -1) ? httpRequestString.length() : endPos;

      messageString = httpRequestString.substring(startPos, endPos);
      messageString.trim();
   }

   return (messageString);
}
