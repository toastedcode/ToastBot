// *****************************************************************************
// *****************************************************************************
// MotorPair.cpp
//
// Author: Jason Tost
// Date:   3.30.2016
//
// *****************************************************************************
// *****************************************************************************

#include "ESP8266.h"
#include "Logger.h"
#include "Messaging.hpp"
#include "MotorPair.hpp"
#include "ToastBot.hpp"

const int MotorPair::MIN_SPEED;

const int MotorPair::MAX_SPEED;

const int MotorPair::MIN_YAW;

const int MotorPair::MAX_YAW;

MotorPair::MotorPair(
   const String& id,
   Motor* leftMotor,
   Motor* rightMotor) : Component(id)
{
   this->leftMotor = leftMotor;
   this->rightMotor = rightMotor;
}

void MotorPair::drive(
   int speed,
   int yaw)
{
   this->speed = constrain(speed, MIN_SPEED, MAX_SPEED);
   this->yaw = constrain(speed, MIN_YAW, MAX_YAW);

   updateMotors();
}

void MotorPair::setup()
{
}

void MotorPair::run()
{

}

void MotorPair::handleMessage(
   MessagePtr message)
{
   // motorPair
   // drive
   if ((message->getMessageId() == "motorPair") ||
       (message->getMessageId() == "drive"))
   {
      Logger::logDebug("MotorPair::handleMessage: motorPair()\n");

      drive(message->getInt("speed"), message->getInt("yaw"));
   }

   Component::handleMessage(message);
}

void MotorPair::updateMotors()
{
   int leftMotorSpeed = speed;
   int rightMotorSpeed = speed;

   if (yaw < 0)
   {
      // Cause the motor pair to yaw to the left by decreasing the speed of the left motor.
      leftMotorSpeed = ((rightMotorSpeed * abs(yaw)) / 100);
   }
   else if (yaw > 0)
   {
      // Cause the motor pair to yaw to the right by decreasing the speed of the right motor.
      rightMotorSpeed = ((leftMotorSpeed * yaw) / 100);
   }

   leftMotor->setSpeed(leftMotorSpeed);
   rightMotor->setSpeed(rightMotorSpeed);
}
