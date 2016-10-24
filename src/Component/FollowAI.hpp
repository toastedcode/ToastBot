#pragma once

#include "Component.hpp"
#include "Logger.hpp"
#include "MotorPair.hpp"
#include "Scanner.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class FollowAI : public Component, TimerListener
{

public:

   FollowAI(
      const String& id,
      Scanner* scanner,
      MotorPair* motorPair);

   virtual ~FollowAI();

   virtual void setup();

   virtual void handleMessage(
      MessagePtr message);

   virtual void timeout(
      Timer* timer);

   void enable();

   void disable();

   bool isEnabled();

   static const int AI_PERIOD;  // milliseconds

private:

   enum Proximity
   {
      NONE,
      ADJACENT,
      NEAR,
      FAR
   };

   enum Bearing
   {
      NORTH_WEST,
      NORTH_NORTH_WEST,
      NORTH,
      NORTH_NORTH_EAST,
      NORTH_EAST
   };

   void getMinDistance(
      const Scanner::ScannerReading& scannerReading,
      int& minDistance,
      int& minPosition);

   Proximity getProximity(
      const Scanner::ScannerReading& scannerReading);

   Bearing getBearing(
      const Scanner::ScannerReading& scannerReading);

   int getSpeed(
      const Scanner::ScannerReading& scannerReading);

   int getYaw(
      const Scanner::ScannerReading& scannerReading);

   Timer* timer;

   MotorPair* motorPair;

   Scanner* scanner;

   bool isAiEnabled;
};

inline bool FollowAI::isEngaged()
{
   return (isAiEngaged);
}

