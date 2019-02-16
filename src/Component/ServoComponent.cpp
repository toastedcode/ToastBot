// *****************************************************************************
// *****************************************************************************
// ServoComponent.cpp
//
// Author: Jason Tost
// Date:   10.7.2016
//
// *****************************************************************************
// *****************************************************************************

#include "../Behavior/ServoPanBehavior.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "ServoComponent.hpp"

const int ServoComponent::MIN_ANGLE;

const int ServoComponent::MAX_ANGLE;

const int ServoComponent::MIN_PWM;

const int ServoComponent::MAX_PWM;

ServoComponent::ServoComponent(
   const String& id,
   const int& pin) :
      Component(id),
      pin(pin),
      limitMin(MIN_ANGLE),
      limitMax(MAX_ANGLE),
      isReversed(false)
{
   servoPanBehavior = new ServoPanBehavior(getId() + ".pan", this);
}

ServoComponent::~ServoComponent()
{
   // Nothing to do here.
}

ServoComponent::ServoComponent(
   MessagePtr message) :
      Component(message)
{
   pin = message->isSet("pin") ? message->getInt("pin") : 0;
   limitMin = message->isSet("limitMin") ? message->getInt("limitMin") : MIN_ANGLE;
   limitMax = message->isSet("limitMax") ? message->getInt("limitMax") : MAX_ANGLE;
   isReversed = message->isSet("isReversed") ? message->getBool("isReversed") : false;
   servoPanBehavior = new ServoPanBehavior(getId() + ".pan", this);
}

void ServoComponent::rotate(
   int angle)
{
   servo.write(transformAngle(angle));
}

void ServoComponent::panTo(
   const int& angle,
   const int& seconds)
{
   servoPanBehavior->panTo(transformAngle(angle), seconds);
}

void ServoComponent::oscillate(
   const int& startAngle,
   const int& endAngle,
   const int& seconds)
{
   servoPanBehavior->oscillate(transformAngle(startAngle), transformAngle(endAngle), seconds);
}

void ServoComponent::stop()
{
   servoPanBehavior->stop();
}

void ServoComponent::limit(
   const int& minAngle,
   const int& maxAngle)
{
   this->limitMin = minAngle;
   this->limitMax = maxAngle;
}

void ServoComponent::reverse(
   const bool& isReversed)
{
   this->isReversed = isReversed;
}

void ServoComponent::setPwm(
   const int& pwm)
{
   servo.writeMicroseconds(constrain(pwm, MIN_PWM, MAX_PWM));
}

void ServoComponent::setup()
{
   servo.attach(pin);

   Messaging::subscribe(this, "killSwitch");

   servoPanBehavior->setup();
}

void ServoComponent::loop()
{
   Component::loop();

   servoPanBehavior->loop();
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
      int angle = message->getInt("angle");

      Logger::logDebug(F("ServoComponent::handleMessage: rotate(%d)"), angle);

      rotate(angle);

      Messaging::freeMessage(message);
   }
   // panTo
   // stop
   else if ((message->getMessageId() == "panTo") ||
            (message->getMessageId() == "oscillate") ||
            (message->getMessageId() == "stop"))
   {
      servoPanBehavior->handleMessage(message);
   }
   // limit
   else if (message->getMessageId() == "limit")
   {
      int newLimitMin = message->getInt("limitMin");
      int newLimitMax = message->getInt("limitMax");

      Logger::logDebug(F("ServoComponent::handleMessage: limit(%d, %d)"), newLimitMin, newLimitMax);

      limit(
         constrain(newLimitMin, MIN_ANGLE, MAX_ANGLE),
         constrain(newLimitMax, MIN_ANGLE, MAX_ANGLE));
   }
   // reverse
   else if (message->getMessageId() == "reverse")
   {
      bool newIsReversed = message->getBool("isReversed");

      Logger::logDebug(F("ServoComponent::handleMessage: reverse(%s)"), (newIsReversed ? "true" : "false"));

      reverse(newIsReversed);
   }
   // setPwm
   else if (message->getMessageId() == "setPwm")
   {
      int pwm = message->getInt("pwm");

      Logger::logDebug(F("ServoComponent::handleMessage: setPwm(%d)"), pwm);

      setPwm(pwm);
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug(F("ServoComponent::handleMessage: killSwitch"));

      rotate(MIN_ANGLE);

      Messaging::freeMessage(message);
   }
   else
   {
      Component::handleMessage(message);
   }
}


int ServoComponent::transformAngle(
   const int& angle)
{
  int transformedAngle = angle;

  if (isReversed)
  {
     transformedAngle = (MAX_ANGLE - angle);
     constrain(transformedAngle, (MAX_ANGLE - limitMax), (MAX_ANGLE - limitMin));
  }
  else
  {
     transformedAngle = constrain(transformedAngle, limitMin, limitMax);
  }

  return (transformedAngle);
}
