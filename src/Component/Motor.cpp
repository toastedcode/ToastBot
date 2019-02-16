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
      speed(0),
      limitMin(MIN_SPEED),
      limitMax(MAX_SPEED),
      isReversed(false)
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
   limitMin = message->isSet("limitMin") ? message->getInt("limitMin") : MIN_SPEED;
   limitMax = message->isSet("limitMax") ? message->getInt("limitMax") : MAX_SPEED;
   isReversed = message->isSet("isReversed") ? message->getBool("isReversed") : false;
}

void Motor::setSpeed(
   int speed)
{
   this->speed = transformSpeed(speed);

   updatePins();
}

void Motor::limit(
   const int& minAngle,
   const int& maxAngle)
{
   this->limitMin = minAngle;
   this->limitMax = maxAngle;
}

void Motor::reverse(
   const bool& isReversed)
{
   this->isReversed = isReversed;
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

      Logger::logDebug(F("Motor::handleMessage: setSpeed(%d)"), newSpeed);

      setSpeed(newSpeed);

      Messaging::freeMessage(message);
   }
   // limit
   else if (message->getMessageId() == "limit")
   {
      int newLimitMin = message->getInt("limitMin");
      int newLimitMax = message->getInt("limitMax");

      Logger::logDebug(F("Motor::handleMessage: limit(%d, %d)"), newLimitMin, newLimitMax);

      limit(
         constrain(newLimitMin, MIN_SPEED, MAX_SPEED),
         constrain(newLimitMax, MIN_SPEED, MAX_SPEED));
   }
   // reverse
   else if (message->getMessageId() == "reverse")
   {
      bool newIsReversed = message->getBool("isReversed");

      Logger::logDebug(F("Motor::handleMessage: reverse(%s)"), (newIsReversed ? "true" : "false"));

      reverse(newIsReversed);
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug(F("Motor::handleMessage: killSwitch"));

      setSpeed(NO_SPEED);

      Messaging::freeMessage(message);
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

int Motor::transformSpeed(
   const int& speed)
{
   const int sDIRECTION = (speed / abs(speed));

   int transformedSpeed = (constrain(abs(transformedSpeed), limitMin, limitMax) * sDIRECTION);

   if (isReversed)
   {
      transformedSpeed = (transformedSpeed * -1);
   }

   return (transformedSpeed);
}
