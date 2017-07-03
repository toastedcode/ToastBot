#pragma once

#include "Component.hpp"
#include "HealthDefs.hpp"
#include "Timer.hpp"

class HealthMonitor : public Component, TimerListener
{

public:

   // Constructor.
   HealthMonitor(
      // A unique identifer for this component.
      const String& id,
      // The adapter which will be health monitored.
      const String& adapterId,
      // Keep-alive heartbeat time (in milliseconds).
      const int& pingRate = 1000,  // 1 second
      // Number of missed pings before the monitored adapter is considered failed.
      const int& failureThreshold = 3);

   // Destructor.
   virtual ~HealthMonitor();

   virtual void handleMessage(
      MessagePtr message);

   virtual void timeout(
      Timer* timer);

   void start();

   void stop();

   void setHealth(
      const Health& health);

   Health getHealth() const;

protected:

   virtual void onSuccessfulPing();

   virtual void onUnsuccessfulPing();

private:

   String adapterId;

   Health health;

   int pingRate;

   int failureThreshold;

   bool gotReply;

   int missedPings;

   int receivedPings;

   Timer* timer;
};
