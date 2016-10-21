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
   const String& id) : Component(id),
                       pollTimer(0)
{
}

Sensor::~Sensor()
{
}

void Sensor::timeout(
   Timer* timer)
{
   read();
}

void Sensor::poll(
   // The rate at which updates should be sent, in milliseconds.
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
