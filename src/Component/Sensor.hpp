// *****************************************************************************
// *****************************************************************************
// Sensor.hpp
//
// Author: Jason Tost
// Date:   10.20.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class Sensor : public Component, TimerListener
{

public:

   // Constructor.
   Sensor(
      // A unique identifier for this sensor.
      const String& id);

   // Destructor.
   virtual ~Sensor();

   virtual void timeout(
      Timer* timer);

   virtual int read() = 0;

   virtual int value() = 0;

   // This operation sets up automatic polling on the sensor.
   void poll(
      // The rate at which updates should be sent, in milliseconds.
      const int& period);

private:

   Timer* pollTimer;
};
