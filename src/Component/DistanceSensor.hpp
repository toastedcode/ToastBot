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
      DISTANCE_UNITS_FIRST,
      MICROSECONDS = DISTANCE_UNITS_FIRST,
      CENTIMETERS,
      INCHES,
      DISTANCE_UNITS_LAST,
      DISTANCE_UNITS_COUNT = DISTANCE_UNITS_LAST - DISTANCE_UNITS_FIRST
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

   // **************************************************************************
   //                           Component interface

   virtual void loop();

   // **************************************************************************
   //                         MessageHandler interface

   // This operation handles a message directed to this component.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // **************************************************************************
   //                           Sensor interface

   // This operation takes a sensor reading and returns the new sensor value.
   virtual int read();

   // **************************************************************************

   // This operation sets the distance units that will be used in calls to read() and poll().
   void setUnits(
      // The units for sensor readings.
      const DistanceUnits& units);

   // This operation takes a number of sensor reading and returns median value.
   virtual int readMedian(
      const int& iterations);

   static int toCentimeters(
      const int& microseconds);

   static int toInches(
      const int& microseconds);

   static int convertUnits(
      const int& microSeconds,
      const DistanceUnits& units);

   static String toString(
      const DistanceUnits& mode);

   static DistanceUnits parseDistanceUnits(
      const String& unitsString);

private:

   NewPing* sensor;

   int sensorValue;

   DistanceUnits units;
};

inline String DistanceSensor::toString(
   const DistanceSensor::DistanceUnits& units)
{
   static const String enumToString[]
   {
      "MICROSECONDS",
      "CENTIMETERS",
      "INCHES",
   };

   return (enumToString[units]);
}

inline DistanceSensor::DistanceUnits DistanceSensor::parseDistanceUnits(
   const String& unitsString)
{
   DistanceUnits units = MICROSECONDS;

   for (int i = DISTANCE_UNITS_FIRST; i < DISTANCE_UNITS_LAST; i++)
   {
      if (unitsString == toString(static_cast<DistanceUnits>(i)))
      {
         units = static_cast<DistanceUnits>(i);
         break;
      }
   }

   return (units);
}

REGISTER(DistanceSensor, distancesensor)
