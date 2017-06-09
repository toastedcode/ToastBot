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
   const int& maxCmDistance) : Sensor(id)
{
   sensor = new NewPing(triggerPin, echoPin, maxCmDistance);
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

int DistanceSensor::value()
{
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
