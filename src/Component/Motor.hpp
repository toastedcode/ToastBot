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
#include "ComponentFactory.hpp"

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

   // Constructor.
   Motor(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~Motor();

   // Sets the speed of the motor.
   void setSpeed(
      // The speed of the motor.
      // Positive values indicate the montor is running forward; negative values reverse.
      int speed);

   void limit(
      const int& minSpeed,
      const int& maxSpeed);

   void reverse(
      const bool& isReversed);

   // Retrieves the speed of the motor.
   int getSpeed() const;

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   static const int NO_SPEED = 0;

   static const int MIN_SPEED = 0;

   static const int MAX_SPEED = 100;

   static const int NO_PWM = 0;

   static const int MIN_PWM = 300;

   static const int MAX_PWM = 1023;

private:

   // Updates the speed and direction GPIO pins to reflect the current motor speed.
   void updatePins();

   // Transforms the speed based on the limitMin, limitMax, and isReversed params.
   int transformSpeed(
      // The angle to transform.
      const int& angle);

   // The GPIO pin attached to the direction pin of the motor.
   int directionPin;

   // The GPIO pin attached to the speed pin of the motor.
   int speedPin;

   // The speed of the motor.
   // Positive values indicate the motor is running forward; negative values reverse.
   int speed;

   // A value limiting the minimum speed (above zero) of the motor.
   // Note: This limit is an absolute value that applies to positive and negative speeds.
   int limitMin;

   // A value limiting the maximum speed of the motor.
   // Note: This limit is an absolute value that applies to positive and negative speeds.
   int limitMax;

   // A flag indicating that all speed parameters should be flipped (i.e. 100 = -100 and -100 = 100).
   bool isReversed;
};

REGISTER(Motor, motor)

inline Motor::~Motor()
{
   // Nothing to do here.
}

inline int Motor::getSpeed() const
{
   return (speed);
}
