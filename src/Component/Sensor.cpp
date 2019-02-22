// *****************************************************************************
// *****************************************************************************
// Sensor.cpp
//
// Author: Jason Tost
// Date:   10.21.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Sensor.hpp"

Sensor::Sensor(
   const String& id) :
      Component(id),
      pollTimer(0)
{
}

Sensor::Sensor(
   MessagePtr message) :
      Component(message),
      pollTimer(0)
{
}

Sensor::~Sensor()
{
}

// **************************************************************************
//                         MessageHandler interface

void Sensor::handleMessage(
   MessagePtr message)
{
   // read
   if (message->getMessageId() == "read")
   {
      Logger::logDebug(F("Sensor::handleMessage: read()"));

      sendReading(message->getSource());
   }
   // poll
   if (message->getMessageId() == "poll")
   {
      int period = message->getInt("period");

      Logger::logDebug(F("Sensor::handleMessage: poll(%d)"), period);

      poll(period);
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug(F("Sensor::handleMessage: killSwitch"));

      poll(0);
   }
   else
   {
      Component::handleMessage(message);
   }

   Messaging::freeMessage(message);
}

// **************************************************************************
//                         TimerListner interface

void Sensor::timeout(
   Timer* timer)
{
   onPoll();
}

// **************************************************************************

void Sensor::onPoll()
{
   broadcastReading();
}

void Sensor::poll(
   const int& period)
{
   if (pollTimer)
   {
      Timer::freeTimer(pollTimer);
   }

   if (period > 0)
   {
      pollTimer = Timer::newTimer("pollTimer", period, Timer::PERIODIC, this);

      if (pollTimer)
      {
         pollTimer->start();
      }
   }
}

void Sensor::sendReading(
      const String& destination)
{
   MessagePtr message = Messaging::newMessage();

   if (message)
   {
      message->setMessageId("sensorReading");
      message->setSource(getId());
      message->set("value", read());

      Messaging::send(message);
   }
}

void Sensor::broadcastReading()
{
   MessagePtr message = Messaging::newMessage();

   if (message)
   {
      message->setTopic(getId());
      message->setMessageId("sensorReading");
      message->setSource(getId());
      message->set("value", read());

      Messaging::publish(message);
   }
}
