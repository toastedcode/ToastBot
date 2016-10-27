#include "FollowAI.hpp"

const int FollowAI::AI_PERIOD = 250;  // milliseconds

FollowAI::FollowAI(
   const String& id,
   Scanner* scanner,
   MotorPair* motorPair) : Component(id),
                           timer(0),
                           scanner(scanner),
                           motorPair(motorPair),
                           isAiEnabled(false)
{
}

FollowAI::~FollowAI()
{
   Timer::freeTimer(timer);
}

void FollowAI::setup()
{
}

void FollowAI::handleMessage(
   MessagePtr message)
{
   // engage
   if (message->getMessageId() == "enable")
   {
      enable();
      message->setFree();
   }
   // disengage
   else if (message->getMessageId() == "disable")
   {
      disable();
      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}

void FollowAI::timeout(
   Timer* timer)
{
   Scanner::ScannerReading reading;

   if (isEnabled())
   {
      scanner->read(reading);

      Proximity proximity = getProximity(reading);
      Bearing bearing = getBearing(reading);
      Logger::logDebug("FollowAI::timeout: Proximity = %d, bearing = %d", proximity, bearing);

      int speed = getSpeed(reading);

      int yaw = getYaw(reading);

      motorPair->drive(speed, yaw);
   }
}

void FollowAI::enable()
{
   isAiEnabled = true;

   if (timer)
   {
      Timer::freeTimer(timer);
   }

   timer = Timer::newTimer("followAiTimer", AI_PERIOD, Timer::PERIODIC, this);

   scanner->enable();
}

void FollowAI::disable()
{
   isAiEnabled = false;

   if (timer)
   {
      Timer::freeTimer(timer);
   }

   scanner->disable();
}

void FollowAI::getMinDistance(
   const Scanner::ScannerReading& scannerReading,
   int& minDistance,
   int& minPosition)
{
   minDistance = 0;
   minPosition = 0;

   for (int i = 0; i < Scanner::SERVO_POSITIONS; i++)
   {
      if ((scannerReading[i] != 0) &&
          ((minDistance == 0) ||
           (scannerReading[i] < minDistance)))
      {
         minDistance = scannerReading[i];
         minPosition = i;
      }
   }
}

FollowAI::Proximity FollowAI::getProximity(
   const Scanner::ScannerReading& scannerReading)
{
   Proximity proximity = NONE;

   static const int ADJACENT_DISTANCE = 20;  // cm
   static const int NEAR_DISTANCE = 50;  // cm

   int minDistance = 0;
   int minPosition = 0;
   getMinDistance(scannerReading, minDistance, minPosition);

   if (minDistance == 0)
   {
      proximity = NONE;
   }
   else if (minDistance <= ADJACENT_DISTANCE)
   {
      proximity = ADJACENT;
   }
   else if (minDistance < NEAR_DISTANCE)
   {
      proximity = NEAR;
   }
   else
   {
      proximity = FAR;
   }

   return (proximity);
}

FollowAI::Bearing FollowAI::getBearing(
   const Scanner::ScannerReading& scannerReading)
{
   Bearing bearing = NORTH;

   int minDistance = 0;
   int minPosition = 0;
   getMinDistance(scannerReading, minDistance, minPosition);

   if (minDistance == 0)
   {
      bearing = NORTH;
   }
   else
   {
      bearing = (Bearing)minPosition;
   }

   return (bearing);
}

int FollowAI::getSpeed(
   const Scanner::ScannerReading& scannerReading)
{
   static const int FOLLOW_SPEEDS[] =
   {
      0,    // NONE
      0,    // ADJACENT,
      60,   // NEAR,
      100,  // FAR
   };

   Proximity proximity = getProximity(scannerReading);

   return (FOLLOW_SPEEDS[proximity]);
}

int FollowAI::getYaw(
   const Scanner::ScannerReading& scannerReading)
{
   static const int FOLLOW_YAW[] =
   {
      -60,  // NORTH_WEST,
      -30,  // NORTH_NORTH_WEST,
      0,    // NORTH,
      30,   // NORTH_NORTH_EAST,
      60,   // NORTH_EAST
   };

   Bearing bearing = getBearing(scannerReading);

   return (FOLLOW_YAW[bearing]);
}

