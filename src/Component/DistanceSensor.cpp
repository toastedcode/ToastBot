// *****************************************************************************
// *****************************************************************************
// DistanceSensor.cpp
//
// Author: Jason Tost
// Date:   10.21.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "DistanceSensor.hpp"

DistanceSensor::DistanceSensor(
   const String& id,
   const int& triggerPin,
   const int& echoPin,
   const int& maxCmDistance) :
      Sensor(id),
      sensorValue(0),
      pollUnits(MICROSECONDS)
{
   sensor = new NewPing(triggerPin, echoPin, maxCmDistance);
}

DistanceSensor::DistanceSensor(
   MessagePtr message) :
      Sensor(message),
      sensorValue(0),
      pollUnits(MICROSECONDS)
{
   sensor = new NewPing(message->getInt("triggerPin"),
                        message->getInt("echoPin"),
                        message->getInt("maxDistance"));
}

DistanceSensor::~DistanceSensor()
{
   delete (sensor);
}

void DistanceSensor::loop()
{
   Sensor::loop();
}

int DistanceSensor::read()
{
   sensorValue = sensor->ping();

   return (sensorValue);
}

int DistanceSensor::read(
   const DistanceUnits& units)
{
   int sensorValue = read();

   switch (units)
   {
      case CENTIMETERS:
      {
         sensorValue = toCentimeters(sensorValue);
         break;
      }

      case INCHES:
      {
         sensorValue = toInches(sensorValue);
         break;
      }

      case MICROSECONDS:
      default:
      {
         break;
      }
   }

   return (sensorValue);
}

int DistanceSensor::readMedian(
   const int& iterations)
{
   sensorValue = sensor->ping_median(iterations);

   return (sensorValue);
}

int DistanceSensor::toCentimeters(
   const int& microseconds)
{
   return (NewPing::convert_cm(microseconds));
}

int DistanceSensor::toInches(
   const int& microseconds)
{
   return (NewPing::convert_in(microseconds));
}
