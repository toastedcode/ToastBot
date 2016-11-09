#pragma once

#include "Behavior.hpp"
#include "../Component/DistanceSensor.hpp"
#include "../Component/MotorPair.hpp"

class ScoutBehavior : public Behavior, BehaviorListener
{

public:

   enum State
   {
      INIT,
      FORWARD,
      REVERSE,
      ROTATE
   };

   ScoutBehavior(
      const String& id,
      MotorPair* motorPair,
      DistanceSensor* distanceSensor);

   virtual ~ScoutBehavior();

   virtual void handleMessage(
      MessagePtr message);

   virtual void enable();

   virtual void disable();

   virtual void onStateChange(
      Behavior* behavior,
      const int& previousState,
      const int& newState);

   virtual void setState(
      const int& state);

private:

   Behavior* forwardBehavior;

   Behavior* reverseBehavior;

   Behavior* rotateBehavior;
};
