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

// The number of simultaneous timers supported by the Timer framework.
const int TIMER_POOL_SIZE = 10;

WiderFi* widerFi;

void setup()
{
   Serial.begin(9600);
   Logger::setLogger(new SerialLogger());
   Logger::setLogLevel(DEBUG_FINEST);

   // Create the timer pool.
   Timer::setup(TIMER_POOL_SIZE);
   
   widerFi = new WiderFi();
   widerFi->setup();
}

void loop()
{
   widerFi->loop();
}

