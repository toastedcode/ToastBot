#include "Logger.hpp"
#include "EscapeBehavior.hpp"
#include "ScoutBehavior.hpp"

static const int DISTANCE_THRESHOLD = 10;

static const int FORTY_FIVE_DEGREE_ROTATE_TIME = 1000;  // milliseconds

static const int ONE_HUNDRED_EIGHTY_DEGREE_ROTATE_TIME = 2000;  // milliseconds

static const int READ_SENSOR_TIME = 500;  // milliseconds

static const int FLEE_TIME = 5000;  // milliseconds

EscapeBehavior::EscapeBehavior(
   const String& id,
   MotorPair* motorPair,
   DistanceSensor* distanceSensor,
   ServoComponent* servo) :
      Behavior(id),
      motorPair(motorPair),
      distanceSensor(distanceSensor),
	  servo(servo)
{
   fleeBehavior = new ScoutBehavior(id + ".flee", motorPair, distanceSensor, servo);
   addChild(fleeBehavior);
   fleeBehavior->disable();

   watchTimer = Timer::newTimer(getId() + ".watch", READ_SENSOR_TIME, Timer::PERIODIC, this);
   fleeTimer = 0;
   rotateTimer = 0;

   disable();
}

EscapeBehavior::~EscapeBehavior()
{
   delete (fleeBehavior);
}

void EscapeBehavior::enable()
{
   setState(WATCH);
   Behavior::enable();
}

void EscapeBehavior::disable()
{
   setState(INIT);
   Behavior::disable();
}

void EscapeBehavior::setState(
   const int& state)
{

   Logger::logDebug("EscapeBehavior::setState: %s -> %d", getId().c_str(), state);

   switch (state)
   {
      case INIT:
      {
         fleeBehavior->setState(ScoutBehavior::INIT);
         watchTimer->stop();
         if (rotateTimer)
         {
            rotateTimer->stop();
         }
         break;
      }

      case WATCH:
      {
         //fleeBehavior->disable();
         watchTimer->start();
         break;
      }

      case FLEE:
      {
         watchTimer->stop();
         fleeTimer = Timer::newTimer(getId() + ".flee", FLEE_TIME, Timer::ONE_SHOT, this);
         fleeTimer->start();
         rotate(ONE_HUNDRED_EIGHTY_DEGREE_ROTATE_TIME);
         break;
      }
   }

   Behavior::setState(state);
}

void EscapeBehavior::timeout(
   Timer* timer)
{
   Logger::logDebug("EscapeBehavior::timeout: %s", timer->getId().c_str());

   if ((getState() == WATCH) &&
       (timer == watchTimer))
   {
      int reading = DistanceSensor::toCentimeters(distanceSensor->read());
      Logger::logDebug("EscapeBehavior::timeout: reading = %d", reading);

      if ((reading != 0) && (reading < DISTANCE_THRESHOLD))
      {
         setState(FLEE);
      }
      else
      {
         // Randomly, rotate.
         if (random(100) < 25)
         {
            rotate(FORTY_FIVE_DEGREE_ROTATE_TIME);
         }
      }
   }
   else if ((getState() == FLEE) &&
            (timer == fleeTimer))
   {
      setState(WATCH);
   }
   else if (timer == rotateTimer)
   {
      motorPair->rotate(0);

      if (getState() == FLEE)
      {
         fleeBehavior->enable();
      }
   }
}

void EscapeBehavior::rotate(
   const int& rotateTime)
{
   motorPair->rotate(100);
   rotateTimer = Timer::newTimer(getId() + ".rotate", rotateTime, Timer::ONE_SHOT, this);
   rotateTimer->start();
}
