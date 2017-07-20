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

#include "Robox.h"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());

   ToastBot::addComponent(new Robox(), true);  // <-- default handler

   // TEST CODE
   MotorPair* motorPair = (MotorPair*)ToastBot::getComponent("motorPair");
   DistanceSensor* distance = (DistanceSensor*)ToastBot::getComponent("distanceSensor");
   ServoComponent* servo = (ServoComponent*)ToastBot::getComponent("servo1");

   if (motorPair && distance && servo)
   {
      ToastBot::addComponent(new ScoutBehavior("scout", motorPair, distance, servo));
      ((ScoutBehavior*)ToastBot::getComponent("scout"))->enable();
      
      //ToastBot::addComponent(new EscapeBehavior("escape", motorPair, distance, servo));
      //((EscapeBehavior*)ToastBot::getComponent("escape"))->enable();
   }
}

void loop()
{
   ToastBot::loop();
}
