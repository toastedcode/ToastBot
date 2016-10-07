// *****************************************************************************
// *****************************************************************************
// ServoComponent.cpp
//
// Author: Jason Tost
// Date:   10.7.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Logger.h"
#include "ServoComponent.hpp"

const int ServoComponent::MIN_ANGLE;

const int ServoComponent::MAX_ANGLE;

void ServoComponent::rotate(
   int angle)
{
   this->angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
   servo.write(angle);
}

void ServoComponent::handleMessage(
   MessagePtr message)
{
   bool handled = false;

   // servo
   // rotate
   if ((message->getMessageId() == "servo") ||
       (message->getMessageId() == "rotate"))
   {
      int newAngle = message->getInt("angle");

      Logger::logDebug("ServoComponent::handleMessage: rotate(%d)", newAngle);

      rotate(newAngle);
   }

   Component::handleMessage(message);
}
