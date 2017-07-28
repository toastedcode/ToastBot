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

   Logger::setLogLevel(DEBUG_FINEST);

   HttpServerAdapter* httpServerAdapter = new HttpServerAdapter("http");
   ToastBot::addComponent(httpServerAdapter);
}

void loop()
{
   ToastBot::loop();
}
