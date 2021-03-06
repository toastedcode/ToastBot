#pragma once

#include "../Component/ServoComponent.hpp"
#include "Behavior.hpp"

// Forward declaration.
class ServoComponent;

class ServoPanBehavior : public Behavior
{

public:

   enum State
   {
      INIT,
      PAN,
      OSCILLATE,
      DONE
   };

   ServoPanBehavior(
      const String& id,
      ServoComponent* servo);

   virtual ~ServoPanBehavior();

   virtual void handleMessage(
      MessagePtr message);

   virtual void loop();

   void panTo(
      const int& angle,
      const int& seconds);

   void oscillate(
      const int& startAngle,
	  const int& endAngle,
      const int& seconds);

   void stop();

private:

   ServoComponent* servo;

   int startPwm;

   int endPwm;

   unsigned long startTime;

   unsigned long endTime;
};

