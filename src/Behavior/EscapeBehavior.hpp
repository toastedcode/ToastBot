#pragma once

#include "Behavior.hpp"
#include "../Component/DistanceSensor.hpp"
#include "../Component/MotorPair.hpp"
#include "../Component/ServoComponent.hpp"

class EscapeBehavior : public Behavior, TimerListener
{

public:

   enum State
   {
      INIT,
      WATCH,
      FLEE
   };

   EscapeBehavior(
      const String& id,
      MotorPair* motorPair,
      DistanceSensor* distanceSensor,
	  ServoComponent* servo);

   virtual ~EscapeBehavior();

   virtual void enable();

   virtual void disable();

   virtual void setState(
      const int& state);

   virtual void timeout(
      Timer* timer);

private:

   void rotate(
      const int& rotateTime);

   MotorPair* motorPair;

   DistanceSensor* distanceSensor;

   ServoComponent* servo;

   Behavior* fleeBehavior;

   Timer* watchTimer;

   Timer* fleeTimer;

   Timer* rotateTimer;
};
