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

#include "ConfigPage.h"
#include "WiderFi.h"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());

   ToastBot::addComponent(new WiderFi(), true);  // <-- default handler

   Logger::setLogLevel(DEBUG_FINEST);

   WebServerAdapter* webServerAdapter = new WebServerAdapter("web", 80);
   ToastBot::addComponent(webServerAdapter);
   webServerAdapter->addPage(new ConfigPage());
}

void loop()
{
   ToastBot::loop();
}

