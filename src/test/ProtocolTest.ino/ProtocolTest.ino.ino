#include "ToastBot.h"
#include "Message.hpp"

void jsonProtocolTest()
{
   Serial.println("\n\n****************** JsonProtocolTest ******************");
   
   Message message;

   String parsedMessage = "{\"messageId\":\"rotate\", \"source\":\"someAdapter\", \"destination\":\"servo1\", \"angle\":90, \"isClockwise\":true}";

   JsonProtocol jsonProtocol;
   
   jsonProtocol.parse(parsedMessage, &message);
   
   Serial.printf("messageId = %s, source = %s, destination = %s, angle = %d, isClockwise = %s\n",
                 message.getMessageId().c_str(),
                 message.getSource().c_str(),
                 message.getDestination().c_str(),
                 message.getInt("angle"), 
                 message.getBool("isClockwise") ? "true" : "false");

   String serializedMessage = jsonProtocol.serialize(&message);

   Serial.printf("Json serialized: %s\n", serializedMessage.c_str());
}

void restfulProtocolTest()
{
   Serial.println("\n\n****************** RestfulProtocolTest ******************");
   
   Message message;

   String parsedMessage = "/servo1/rotate?angle=90&isClockwise=true";

   RestfulProtocol restfulProtocol;
   
   restfulProtocol.parse(parsedMessage, &message);
   
   Serial.printf("messageId = %s, source = %s, destination = %s, angle = %d, isClockwise = %s\n",
                 message.getMessageId().c_str(),
                 message.getSource().c_str(),
                 message.getDestination().c_str(),
                 message.getInt("angle"), 
                 message.getBool("isClockwise") ? "true" : "false");

   String serializedMessage = restfulProtocol.serialize(&message);

   Serial.printf("Restful serialized: %s\n", serializedMessage.c_str());  
}

void commandProtocolTest()
{
   Serial.println("\n\n****************** CommandProtocolTest ******************");
   
   Message message;

   String parsedMessage = "servo1.rotate(90, true)";

   CommandProtocol commandProtocol;
   
   commandProtocol.parse(parsedMessage, &message);
   
   Serial.printf("messageId = %s, action = %s, source = %s, destination = %s, angle = %d, isClockwise = %s\n",
                 message.getMessageId().c_str(),
                 message.getString("action").c_str(),
                 message.getSource().c_str(),
                 message.getDestination().c_str(),
                 message.getInt("param_0"), 
                 message.getBool("param_1") ? "true" : "false");

   String serializedMessage = commandProtocol.serialize(&message);

   Serial.printf("Command serialized: %s\n", serializedMessage.c_str());  
}

void stressTest(const int& iterations)
{
   JsonProtocol jsonProtocol;

   String parsedMessage = "{\"messageId\":\"rotate\", \"source\":\"someAdapter\", \"destination\":\"servo1\", \"angle\":90, \"isClockwise\":true}";

   String serializedMessage = "";

   Message message;
  
   Serial.printf("\n\n****************** stressTest (%d iterations)  ******************\n", iterations);

   Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
   
   for (int i = 0; i < iterations; i++)
   {
      message.initialize();
   
      jsonProtocol.parse(parsedMessage, &message);

      serializedMessage = jsonProtocol.serialize(&message);

      ESP.wdtFeed();
   }

   Serial.printf("Heap after: %d\n", ESP.getFreeHeap());

}

void setup()
{
   Serial.begin(9600);

   jsonProtocolTest();

   restfulProtocolTest();

   commandProtocolTest();

   stressTest(5000);
}

void loop()
{
}
