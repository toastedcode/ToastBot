#pragma once

#include "Component.hpp"
#include "DistanceSensor.hpp"
#include "Logger.hpp"
#include "MotorPair.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class FollowAI_2 : public Component, TimerListener
{

public:

   FollowAI_2(
      const String& id,
      DistanceSensor* scanner,
      MotorPair* motorPair);

   virtual ~FollowAI_2();

   virtual void setup();

   virtual void handleMessage(
      MessagePtr message);

   virtual void timeout(
      Timer* timer);

   void enable();

   void disable();

   bool isEnabled();

private:

   // **************************************************************************
   // FSM logic

   enum Proximity
   {
      NONE,
      ADJACENT,
      NEAR,
      FAR
   };

   enum State
   {
      WAITING,
      FOLLOWING,
      LOST
   };

   enum SearchDirection
   {
      LEFT,
      RIGHT
   };

   void setState(
      const State& state);

   // Updates the FSM based on a change in proximity.
   void onProximity(
      const Proximity& proximity);

   // Updates the FSM based on the "reading" timeout.
   // This is a cue to take a sensor reading and update the proximity.
   void onReadingTimeout();

   // Updates the FSM based on the "waiting" timeout.
   // This is a cue to rotate to a new position to look for a new leader.
   void onWaitingTimeout();

   // Updates the FSM based on the "lost" timeout.
   // This is a cue to stop looking and start waiting for a new leader.
   void onLostTimeout();

   // Updates the FSM based on the "search" timeout.
   // This is a cue to switch directions in the search for a new leader.
   void onSearchTimeout();

   // **************************************************************************
   // Timers

   void setReadingTimer(
      const bool& isOn);

   void setWaitingTimer(
      const bool& isOn);

   void setLostTimer(
      const bool& isOn);

   void setSearchTimer(
      const bool& isOn);

   // **************************************************************************

   Proximity getProximity(
      const int& reading);

   int getSpeed(
      const Proximity& proximity);

   int getYaw(
      const SearchDirection& searchDirection);

   MotorPair* motorPair;

   DistanceSensor* sensor;

   Timer* readingTimer;

   Timer* waitingTimer;

   Timer* lostTimer;

   Timer* searchTimer;

   bool isAiEnabled;

   State state;

   SearchDirection searchDirection;

   Proximity proximity;
};

inline bool FollowAI_2::isEnabled()
{
   return (isAiEnabled);
}

