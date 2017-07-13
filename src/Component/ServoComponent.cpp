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
      pin(pin)
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
   servoPanBehavior = new ServoPanBehavior(getId() + ".pan", this);
}

void ServoComponent::rotate(
   int angle)
{
   servo.write(constrain(angle, MIN_ANGLE, MAX_ANGLE));
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

      Logger::logDebug("ServoComponent::handleMessage: rotate(%d)", angle);

      rotate(angle);

      message->setFree();
   }
   // panTo
   // stop
   else if ((message->getMessageId() == "panTo") ||
            (message->getMessageId() == "stop"))
   {
      servoPanBehavior->handleMessage(message);
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug("ServoComponent::handleMessage: killSwitch");

      rotate(MIN_ANGLE);

      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}
