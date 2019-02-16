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
      sensorValue(0)
{
   sensor = new NewPing(triggerPin, echoPin, maxCmDistance);
}

DistanceSensor::DistanceSensor(
   MessagePtr message) :
      Sensor(message),
      sensorValue(0)
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
