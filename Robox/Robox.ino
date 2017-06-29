#include <Arduino.h>
#include <Board.h>
#include <ESP8266WiFi.h>
#include <Common.h>
#include <FS.h>
#include <Messaging.h>
#include <NewPing.h>
#include <Logger.h>
#include <Properties.h>
#include <PubSubClient.h>
#include <Timer.h>
#include <ToastBot.h>
#include <WebSocketServer.h>

#include "FactoryResetButton.hpp"
#include "Robox.h"

// TODO: Move to ToastBot
void createComponents()
{
   ToastBot::addComponent(new Robox(), true);  // <-- default handler

   /*
   Motor* motor1 = new Motor("motor1", 0, 5);
   Motor* motor2 = new Motor("motor2", 2, 4);
   ToastBot::addComponent(motor1);
   ToastBot::addComponent(motor2);
      
   MotorPair* motorPair1 = new MotorPair("motorPair1", motor1, motor2);
   ToastBot::addComponent(motorPair1);
   */

   Logger::logDebug("***Creating***");
   Message* message = MessageFactory::newMessage();
   JsonProtocol protocol;
   String componentIds[50];
   int count = 0;
   ToastBot::getProperties().getKeys("component", componentIds, count);
   for (int i = 0; i < count; i++)
   {
      String componentDescription = ToastBot::getProperties().getString(componentIds[i]);
     
      if (protocol.parse(componentDescription, message))
      {
        Logger::logDebug("Creating %s component [%s]", message->getString("class").c_str(), message->getString("id").c_str());

        Component* component = ComponentFactory::create(message->getString("class"), message);
        if (component)
        {
          ToastBot::addComponent(component);
        }
      }
   }
   message->setFree();

   ServoComponent* servo1 = new ServoComponent("servo1", 14);
   ToastBot::addComponent(servo1);
   ToastBot::addComponent(new ServoComponent("servo2", 12));

   DistanceSensor* distance1 = new DistanceSensor("distance1", 13, 15, 200);
   ToastBot::addComponent(distance1);

   Button* flashButton = new FactoryResetButton("flashButton", 0);
   flashButton->setLongPress(5000);
   ToastBot::addComponent(flashButton);

   Led* led1 = new Led("led1", 16);
   ToastBot::addComponent(led1);
   Connection::setStatusLed(led1);

   ToastBot::addComponent(new SerialAdapter("serial", new JsonProtocol()));
   ToastBot::addComponent(new UdpAdapter("discover", new JsonProtocol(), 1993));  
   ToastBot::addComponent(new TcpServerAdapter("control", new JsonProtocol(), 1975));
   ToastBot::addComponent(new TcpServerAdapter("debug", new JsonProtocol(), 1977));

   // Experimental components.   
   /*
   Scanner* scanner1 = new Scanner("scanner1", servo1, distance1);
   ToastBot::addComponent(scanner1);

   FollowAI* follow1 = new FollowAI("follow1", scanner1, motorPair1);
   ToastBot::addComponent(follow1);

   FollowAI_2* follow2 = new FollowAI_2("follow2", distance1, motorPair1);
   ToastBot::addComponent(follow2);

   ToastBot::addComponent(new ScoutBehavior("scout1", motorPair1, distance1));
   
   ToastBot::addComponent(new WebSocketAdapter("adapter1", new JsonProtocol(), 81));
   ToastBot::addComponent(new MqttClientAdapter("adapter3", new JsonProtocol(), "broker.mqtt-dashboard.com", 1883, "toastbot1", "", ""));
   ToastBot::addComponent(new UdpAdapter("adapter5", new JsonProtocol(), IPAddress(10, 1, 11, 249), 55056));
   ToastBot::addComponent(new TcpClientAdapter("adapter5", new JsonProtocol(), "10.1.11.249", 1997));
   */
}

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());
   
   createComponents();
}

void loop()
{
   ToastBot::loop();
}
