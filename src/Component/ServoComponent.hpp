// *****************************************************************************
// *****************************************************************************
// ServoComponent.hpp
//
// Author: Jason Tost
// Date:   10.7.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include <Servo.h>

#include "Component.hpp"

class ServoComponent : public Component
{

public:
   
   // Constructor.
   ServoComponent(
      // A unique identifier for this component.
      const String& id,
      // The control pin for the servo.
      const int& pin);

   // Destructor.
   virtual ~ServoComponent();

   // Sets the speed of the two motors in the pair.
   void rotate(
      // The angle the servo should rotate to.
      int angle);

   // Retrieves the current angle of the servo.
   int getAngle() const;

   // This operation should be called on startup to prepare the sensor for polling/updating.
   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   static const int MIN_ANGLE = 0;

   static const int MAX_ANGLE = 180;

private:

   // The GPIO pin used for controlling the angle of the servo.
   int pin;

   // A servo object from the Arduino library.
   Servo servo;

   // The angle of the servo.
   int angle;
};

inline ServoComponent::ServoComponent(
   const String& id,
   const int& pin) :
      Component(id),
      pin(pin)
{
}

inline void ServoComponent::setup()
{
   servo.attach(pin);
}

inline ServoComponent::~ServoComponent()
{
   // Nothing to do here.
}

inline int ServoComponent::getAngle() const
{
   return (angle);
}
