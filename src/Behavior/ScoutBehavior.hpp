#pragma once

#include "Behavior.hpp"
#include "ComponentFactory.hpp"

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
      const String& motorPairId,
      const String& distanceSensorId,
	   const String& servoId);

   ScoutBehavior(
      MessagePtr message);

   virtual ~ScoutBehavior();

   virtual void setup();

   virtual void enable();

   virtual void disable();

   virtual void onStateChange(
      Behavior* behavior,
      const int& previousState,
      const int& newState);

   virtual void setState(
      const int& state);

private:

   String motorPairId;

   String distanceSensorId;

   String servoId;

   bool initialized;

   Behavior* forwardBehavior;

   Behavior* reverseBehavior;

   Behavior* rotateBehavior;
};

REGISTER(ScoutBehavior, ScoutBehavior)
