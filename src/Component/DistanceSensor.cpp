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
      units(MICROSECONDS)
{
   sensor = new NewPing(triggerPin, echoPin, maxCmDistance);
}

DistanceSensor::DistanceSensor(
   MessagePtr message) :
      Sensor(message),
      sensorValue(0),
      units(MICROSECONDS)
{
   sensor = new NewPing(message->getInt("triggerPin"),
                        message->getInt("echoPin"),
                        message->getInt("maxDistance"));
}

DistanceSensor::~DistanceSensor()
{
   delete (sensor);
}

// **************************************************************************
//                         Component interface

void DistanceSensor::loop()
{
   Sensor::loop();
}

// **************************************************************************
//                        MessageHandler interface

void DistanceSensor::handleMessage(
   MessagePtr message)
{
   // setUnits
   if (message->getMessageId() == "setUnits")
   {
      DistanceUnits newUnits = parseDistanceUnits(message->getString("units"));

      Logger::logDebug(F("DistanceSensor::handleMessage: setUnits(%s)"), toString(newUnits).c_str());

      setUnits(newUnits);

      Messaging::freeMessage(message);
   }
   else
   {
      Sensor::handleMessage(message);
   }
}

// **************************************************************************
//                           Sensor interface

int DistanceSensor::read()
{
   sensorValue = sensor->ping();

   return (convertUnits(sensorValue, units));
}

// **************************************************************************

inline void DistanceSensor::setUnits(
   const DistanceUnits& units)
{
   this->units = units;
}

int DistanceSensor::readMedian(
   const int& iterations)
{
   sensorValue = sensor->ping_median(iterations);

   return (convertUnits(sensorValue, units));
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

int DistanceSensor::convertUnits(
   const int& microSeconds,
   const DistanceUnits& units)
{
   int convertedValue = microSeconds;

   switch (units)
   {
      case CENTIMETERS:
      {
         convertedValue = toCentimeters(microSeconds);
         break;
      }

      case INCHES:
      {
         convertedValue = toInches(microSeconds);
         break;
      }

      case MICROSECONDS:
      default:
      {
         break;
      }
   }

   return (convertedValue);
}
