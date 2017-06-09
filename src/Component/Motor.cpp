// *****************************************************************************
// *****************************************************************************
// Motor.cpp
//
// Author: Jason Tost
// Date:   12.10.2015
//
// *****************************************************************************
// *****************************************************************************

#include "Board.hpp"
#include "Logger.hpp"
#include "Motor.hpp"
#include "Messaging.hpp"

const int Motor::NO_SPEED;

const int Motor::MIN_SPEED;

const int Motor::MAX_SPEED;

const int Motor::NO_PWM;

const int Motor::MIN_PWM;

const int Motor::MAX_PWM;

Motor::Motor(
   const String& id,
   const int& directionPin,
   const int& speedPin) :
      Component(id),
      speed(0)
{
   this->directionPin = directionPin;
   this->speedPin = speedPin;
}

Motor::Motor(
   MessagePtr message) :
      Component(message),
      speed(0)
{
   directionPin = message->isSet("directionPin") ? message->getInt("directionPin") : 0;
   speedPin = message->isSet("speedPin") ? message->getInt("speedPin") : 0;
}

void Motor::setSpeed(
   int speed)
{
   Logger::logDebug("Motor::handleMessage: setSpeed(%d)", speed);
   this->speed = constrain(speed, MIN_SPEED, MAX_SPEED);

   updatePins();
}

void Motor::setup()
{
   if (Board::getBoard())
   {
      Board::getBoard()->pinMode(directionPin, OUTPUT);
      Board::getBoard()->pinMode(speedPin, OUTPUT);
   }

   Messaging::subscribe(this, "killSwitch");
}

void Motor::handleMessage(
   MessagePtr message)
{
   // setSpeed
   if (message->getMessageId() == "setSpeed")
   {
      int newSpeed = message->getInt("speed");

      Logger::logDebug("Motor::handleMessage: setSpeed(%d)", newSpeed);

      setSpeed(newSpeed);

      message->setFree();
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug("Motor::handleMessage: killSwitch");

      setSpeed(NO_SPEED);

      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}

void Motor::updatePins()
{
   if (Board::getBoard())
   {
      if (speed == 0)
      {
         Board::getBoard()->digitalWrite(directionPin, LOW);
         Board::getBoard()->analogWrite(speedPin, NO_PWM);
      }
      else if (speed > 0)
      {
         Board::getBoard()->digitalWrite(directionPin, HIGH);
         Board::getBoard()->analogWrite(speedPin, ((speed * (MAX_PWM - MIN_PWM)) / 100));
      }
      else // if (speed < 0)
      {
         Board::getBoard()->digitalWrite(directionPin, LOW);
         Board::getBoard()->analogWrite(speedPin, ((abs(speed) * (MAX_PWM - MIN_PWM)) / 100));
      }
   }
}
