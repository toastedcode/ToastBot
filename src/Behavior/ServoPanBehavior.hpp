#pragma once

#include "Behavior.hpp"
#include "../Component/ServoComponent.hpp"

class ServoPanBehavior : public Behavior
{

public:

   enum State
   {
      INIT,
      PAN,
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

   void stop();

private:

   ServoComponent* servo;

   int startPwm;

   int endPwm;

   unsigned long startTime;

   unsigned long endTime;
};

