// *****************************************************************************
// *****************************************************************************
// Motor.h
//
// Author: Jason Tost
// Date:   9.29.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"

class Motor : public Component
{

public:
   
   // Constructor.
   Motor(
      // A unique identifer for this sensor.
      const String& id,
      // The GPIO pin that will be used to set the motor direction.
      const int& directionPin,
      // The GPIO pin that will be used to set the motor speed.
      const int& speedPin);

   // Destructor.
   virtual ~Motor();

   // Sets the speed of the motor.
   void setSpeed(
      // The speed of the motor.
      // Positive values indicate the montor is running forward; negative values reverse.
      int speed);

   // Retrieves the speed of the motor.
   int getSpeed() const;

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   static const int NO_SPEED = 0;

   static const int MIN_SPEED = -100;

   static const int MAX_SPEED = 100;

   static const int NO_PWM = 0;

   static const int MIN_PWM = 300;

   static const int MAX_PWM = 1023;

private:

   // Updates the speed and direction GPIO pins to reflect the current motor speed.
   void updatePins();

   // The GPIO pin attached to the direction pin of the motor.
   int directionPin;

   // The GPIO pin attached to the speed pin of the motor.
   int speedPin;

   // The speed of the motor.
   // Positive values indicate the motor is running forward; negative values reverse.
   int speed;
};

inline Motor::~Motor()
{
   // Nothing to do here.
}

inline int Motor::getSpeed() const
{
   return (speed);
}
