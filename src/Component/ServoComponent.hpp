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

#ifdef ESP32
   #include <ESP32_Servo.h>
#else
   #include <Servo.h>
#endif

#include "Component.hpp"
#include "ComponentFactory.hpp"

// Forward declaration.
class ServoPanBehavior;

class ServoComponent : public Component
{

public:
   
   static int angleToPwm(
      const int& angle);

   // Constructor.
   ServoComponent(
      // A unique identifier for this component.
      const String& id,
      // The control pin for the servo.
      const int& pin);

   // Constructor.
   ServoComponent(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~ServoComponent();

   // Sets the speed of the two motors in the pair.
   void rotate(
      // The angle the servo should rotate to.
      int angle);

   void panTo(
      const int& angle,
      const int& seconds);

   void oscillate(
      const int& startAngle,
	  const int& endAngle,
      const int& seconds);

   void stop();

   // Retrieves the current angle of the servo.
   // Note: Non-const because it calls read() on the Servo object.
   int getAngle();

   // Sets the PWM of the servo in milliseconds.
   void setPwm(
      // The new PWM to write to the servo.
      const int& pwm);

   // This operation should be called on startup to prepare the sensor for polling/updating.
   virtual void setup();

   virtual void loop();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   static const int MIN_ANGLE = 0;

   static const int MAX_ANGLE = 180;

   static const int MIN_PWM = 500;

   static const int MAX_PWM = 2200;

private:

   // The GPIO pin used for controlling the angle of the servo.
   int pin;

   // A servo object from the Arduino library.
   Servo servo;

   ServoPanBehavior* servoPanBehavior;
};

REGISTER(ServoComponent, servo)

inline int ServoComponent::angleToPwm(
   const int& angle)
{
   int pwm = 0;

   if ((angle >= MIN_ANGLE) && (angle <= MAX_ANGLE))
   {
      pwm = map(angle, MIN_ANGLE, MAX_ANGLE, MIN_PWM, MAX_PWM);
   }

   return (pwm);
}

inline int ServoComponent::getAngle()
{
   return (servo.read());
}
