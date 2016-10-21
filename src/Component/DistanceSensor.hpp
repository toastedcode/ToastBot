// *****************************************************************************
// *****************************************************************************
// DistanceSensor.hpp
//
// Author: Jason Tost
// Date:   10.21.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "NewPing.h"
#include "Sensor.hpp"

class DistanceSensor : public Sensor
{

public:

   // Constructor.
   DistanceSensor(
      // A unique identifier for this sensor.
      const String& id,
      const int& triggerPin,
      const int& echoPin,
      const int& maxCmDistance);

   // Destructor.
   virtual ~DistanceSensor();

   virtual void loop();

   // This operation takes a sensor reading and returns the new sensor value.
   virtual int read();

   // This operation takes a number of sensor reading and returns median value.
   virtual int readMedian(
      const int& iterations);

   // This operation retrieves the last-read sensor value.
   virtual int value();

   // This operation sets up automatic polling on the sensor.
   void poll(
      // The rate at which updates should be sent, in milliseconds.
      const int& period);

   static int toCentimeters(
      const int& microseconds);

   static int toInches(
      const int& microseconds);

private:

   NewPing* sensor;

   int sensorValue;
};
