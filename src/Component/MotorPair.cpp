// *****************************************************************************
// *****************************************************************************
// MotorPair.cpp
//
// Author: Jason Tost
// Date:   3.30.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Logger.h"
#include "Messaging.hpp"
#include "MotorPair.hpp"

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
   if (message->getMessageId() == "ping")
   {
      Logger::logDebug("MotorPair::handleMessage: ping()\n");

      Message* reply = Messaging::newMessage();
      reply->setMessageId("pong");
      reply->setSource(getId());
      reply->setDestination(message->getSource());
      Messaging::send(reply);
   }
   else if (message->getMessageId() == "motorPair")
   {
      Logger::logDebug("MotorPair::handleMessage: motorPair()\n");

      drive(message->getInt("speed"), message->getInt("yaw"));
   }
   else
   {
      Logger::logDebug("MotorPair::handleMessage: Unhandled message \"" + message->getMessageId() + "\"\n");
   }

   message->setFree();
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
