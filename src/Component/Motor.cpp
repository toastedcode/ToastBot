// *****************************************************************************
// *****************************************************************************
// Motor.cpp
//
// Author: Jason Tost
// Date:   12.10.2015
//
// *****************************************************************************
// *****************************************************************************

#include "ESP8266.h"
#include "Motor.hpp"

#include "Logger.h"
#include "Messaging.hpp"

const int Motor::MIN_SPEED;

const int Motor::MAX_SPEED;

Motor::Motor(
   const String& id,
   const int& directionPin,
   const int& speedPin) : Component(id)
{
   this->directionPin = directionPin;
   this->speedPin = speedPin;
}

void Motor::setSpeed(
   int speed)
{
   this->speed = speed;

   updatePins();
}

void Motor::setup()
{
   pinMode(directionPin, OUTPUT);
   pinMode(speedPin, OUTPUT);
}

void Motor::loop()
{

}

void Motor::handleMessage(
   MessagePtr message)
{
   // MOTOR_CONFIG
   if (message->getMessageId() == "setSpeed")
   {
      int newSpeed = message->getInt("speed");

      Logger::logDebug("Motor::handleMessage: setSpeed(" + String(newSpeed) +  ")\n");

      setSpeed(newSpeed);
   }
   else if (message->getMessageId() == "ping")
   {
      Logger::logDebug("Motor::handleMessage: ping()\n");

      Message* reply = Messaging::newMessage();
      reply->setMessageId("pong");
      reply->setSource(getId());
      reply->setDestination(message->getSource());
      Messaging::send(reply);
   }
   else
   {
      Logger::logDebug("Motor::handleMessage: Unhandled message \"" + message->getMessageId() + "\"\n");
   }

   message->setFree();
}

void Motor::updatePins()
{
   if (speed == 0)
   {
      digitalWrite(directionPin, LOW);
      analogWrite(speedPin, MIN_SPEED);
   }
   else if (speed > 0)
   {
      digitalWrite(directionPin, HIGH);
      analogWrite(speedPin, (MAX_SPEED - speed));
   }
   else // if (speed < 0)
   {
      digitalWrite(directionPin, LOW);
      analogWrite(speedPin, abs(speed));
   }
}
