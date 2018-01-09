// *****************************************************************************
// *****************************************************************************
// MotorPair.cpp
//
// Author: Jason Tost
// Date:   3.30.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Logger.hpp"
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
   Motor* rightMotor) :
      Component(id),
      speed(0),
      yaw(0)
{
   this->leftMotor = leftMotor;
   this->rightMotor = rightMotor;
}

MotorPair::MotorPair(
   MessagePtr message) :
      Component(message),
      leftMotor(0),
      rightMotor(0),
      speed(0),
      yaw(0)
{
   if (message->isSet("leftMotor"))
   {
      leftMotor = (Motor*)ToastBot::getComponent(message->getString("leftMotor"));
   }

   if (message->isSet("rightMotor"))
   {
      rightMotor = (Motor*)ToastBot::getComponent(message->getString("rightMotor"));
   }
}

void MotorPair::drive(
   int speed,
   int yaw)
{
   this->speed = constrain(speed, MIN_SPEED, MAX_SPEED);
   this->yaw = constrain(yaw, MIN_YAW, MAX_YAW);

   updateMotors();
}

void MotorPair::rotate(
   int speed)
{
   this->speed = abs(speed);
   this->yaw = 0;

   if (speed == 0)
   {
      leftMotor->setSpeed(this->speed);
      rightMotor->setSpeed(this->speed);
   }
   else if (speed < 0)
   {
      leftMotor->setSpeed(this->speed * -1);
      rightMotor->setSpeed(this->speed);
   }
   else
   {
      leftMotor->setSpeed(this->speed);
      rightMotor->setSpeed(speed * -1);
   }
}

void MotorPair::setup()
{
   Messaging::subscribe(this, "killSwitch");
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
      Logger::logDebug(F("MotorPair::handleMessage: drive()"));

      drive(message->getInt("speed"), message->getInt("yaw"));

      Messaging::freeMessage(message);
   }
   else if (message->getMessageId() == "rotate")
   {
      Logger::logDebug(F("MotorPair::handleMessage: rotate()\n"));

      rotate(message->getInt("speed"));

      Messaging::freeMessage(message);
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug(F("MotorPair::handleMessage: killSwitch"));

      drive(0, 0);

      Messaging::freeMessage(message);
   }
   else if (message->getMessageId() == "instruction")
   {
      String action = message->getString("action");

      if (action == "drive")
      {
         int speed = message->getInt("param_0");

         Logger::logDebug(F("MotorPair::handleMessage: instruction:drive(%d)"), speed);

         drive(speed, 0);
      }
      else if (action == "rotate")
      {
         int angle = message->getInt("param_0");

         Logger::logDebug(F("MotorPair::handleMessage: instruction:rotate(%d)"), angle);

         rotate(angle);
      }
      else
      {
         Logger::logWarning(F("MotorPair::handleMessage: Illegal instruction [%s] for %s."),
                            message->getString("action").c_str(),
                            getId().c_str());
      }

      Messaging::freeMessage(message);
   }
   else
   {
      Component::handleMessage(message);
   }
}

void MotorPair::updateMotors()
{
   int leftMotorSpeed = speed;
   int rightMotorSpeed = speed;

   if (yaw < 0)
   {
      // Cause the motor pair to yaw to the left by decreasing the speed of the left motor.
      leftMotorSpeed = ((speed * (MAX_YAW - abs(yaw))) / 100);
   }
   else if (yaw > 0)
   {
      // Cause the motor pair to yaw to the right by decreasing the speed of the right motor.
      rightMotorSpeed = ((speed * (MAX_YAW - yaw)) / 100);
   }

   leftMotor->setSpeed(leftMotorSpeed);
   rightMotor->setSpeed(rightMotorSpeed);
}
