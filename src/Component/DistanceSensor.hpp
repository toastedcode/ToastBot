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

#include "ComponentFactory.hpp"
#include "NewPing.h"
#include "Sensor.hpp"

class DistanceSensor : public Sensor
{

public:

   enum DistanceUnits
   {
      MICROSECONDS,
      CENTIMETERS,
      INCHES
   };

   // Constructor.
   DistanceSensor(
      // A unique identifier for this sensor.
      const String& id,
      const int& triggerPin,
      const int& echoPin,
      const int& maxCmDistancDistanceUnitse);

   // Constructor.
   DistanceSensor(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~DistanceSensor();

   virtual void loop();

   // This operation takes a sensor reading and returns the new sensor value.
   virtual int read();

   // This operation takes a sensor reading and returns the new sensor value.
   int read(
      // The units for the sensor reading.
      const DistanceUnits& units);

   // This operation takes a number of sensor reading and returns median value.
   virtual int readMedian(
      const int& iterations);

   // This operation sets up automatic polling on the sensor.
   void poll(
      // The rate at which updates should be sent, in milliseconds.
      const int& period,
      // The units for sensor reading broadcasts.
      const DistanceUnits& units = MICROSECONDS);

   static int toCentimeters(
      const int& microseconds);

   static int toInches(
      const int& microseconds);

protected:

   virtual void onPoll();

private:

   NewPing* sensor;

   int sensorValue;

   DistanceUnits pollUnits;
};

REGISTER(DistanceSensor, distancesensor)
