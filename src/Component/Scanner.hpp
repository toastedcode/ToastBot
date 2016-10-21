#pragma once

#include "Component.hpp"
#include "DistanceSensor.hpp"
#include "ServoComponent.hpp"
#include "Timer.hpp"

class Scanner : public Component, TimerListener
{

public:

   static const int SERVO_POSITIONS = 5;

   static const int MIN_SERVO_POSITION = 0;

   static const int MAX_SERVO_POSITION = SERVO_POSITIONS - 1;

   static const int SERVO_ROTATE_PERIOD = 1000;  // milliseconds

   typedef int ScannerReading[SERVO_POSITIONS];

   Scanner(
      const String& id,
      ServoComponent* servo,
      DistanceSensor* sensor);

   ~Scanner();

   virtual void setup();

   virtual void handleMessage(
      MessagePtr message);

   virtual void timeout(
      Timer* timer);

   void read(
      ScannerReading& reading);

   static int getServoAngle(
      const int& position);

private:

   enum ServoDirection
   {
      RIGHT,
      LEFT
   };

   ServoComponent* servo;

   DistanceSensor* sensor;

   int servoPosition;

   ServoDirection servoDirection;

   ScannerReading reading;

   Timer* servoTimer;
};
