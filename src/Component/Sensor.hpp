// *****************************************************************************
// *****************************************************************************
// AnalogInput.hpp
//
// Author: Jason Tost
// Date:   10.12.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Messaging.hpp"

class Sensor : public Component
{

public:

   // Constructor.
   Sensor(
      // A unique identifier for this sensor.
      const String& id,
      // The GPIO pin that will be used by this sensor.
      const int& pinId);

   // Destructor.
   virtual ~Sensor();

   // This operation should be called on startup to prepare the sensor for polling/updating.
   virtual void setup() = 0;

   // This operation should be called continuously from within the main control loop.
   virtual void run() = 0;

   // This operation retrieves the
   virtual int read();

   // This operation sets up automatic polling on the sensor.
   void poll(
      // The rate at which updates should be sent, in milliseconds.
      const int& pollRate,
      // The topic under which the sensor will send updates.
      const String& topic);

protected:

   // The pin that will be used to for reading from the sensor.
   int pinId;
};
