#pragma once

#include "Component.hpp"
#include "Timer.hpp"

class KillSwitch : public Component, TimerListener
{

public:

   // Constructor.
   KillSwitch(
      // A unique identifer for this component.
      const String& id);

   // Destructor.
   virtual ~KillSwitch();

   // Resets the timer in the kill switch.
   // Call this periodically to keep everything alive.
   void reset();

   virtual void setup();

   virtual void timeout(
      Timer* timer);

private:

   Timer* timer;
};
