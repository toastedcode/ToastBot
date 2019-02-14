#include "Logger.hpp"
#include "ScoutBehavior.hpp"
#include "ToastBot.hpp"
#include "../Component/DistanceSensor.hpp"
#include "../Component/MotorPair.hpp"
#include "../Component/ServoComponent.hpp"

// *****************************************************************************

namespace
{

class ForwardBehavior : public Behavior, TimerListener
{

public:

   enum State
   {
      INIT,
      MOVING,
      BLOCKED
   };

   ForwardBehavior(
      const String& id,
      MotorPair* motorPair,
      DistanceSensor* distanceSensor,
	   ServoComponent* servo) :
         Behavior(id)
   {
      this->motorPair = motorPair;
      this->distanceSensor = distanceSensor;
      this->servo = servo;

      timer = Timer::newTimer("sensorTimer", 500, Timer::PERIODIC, this);

      setState(INIT);
   }

   ~ForwardBehavior()
   {
      Timer::freeTimer(timer);
   }

   virtual void timeout(
      Timer* timer)
   {
	  static const int NO_READING = 0;

	  static const int DISTANCE_THRESHOLD = 20;  // cm

      if (isEnabled())
      {
         int reading = DistanceSensor::toCentimeters(distanceSensor->read());

         Logger::logDebug(F("ForwardBehavior::timeout: %d\n"), reading);

         if ((getState() == MOVING) &&
             (reading != NO_READING) &&
			 (reading <= DISTANCE_THRESHOLD))
         {
            setState(BLOCKED);
         }
      }
   }

   virtual void setState(
      const int& state)
   {
      Logger::logDebug(F("ForwardBehavior::setState: %s -> %d"), getId().c_str(), state);

      switch (state)
      {
         case INIT:
         {
            motorPair->drive(0, 0);
            servo->stop();
            servo->rotate(90);
            timer->stop();
            break;
         }

         case MOVING:
         {
            motorPair->drive(100, 0);
            servo->oscillate(0, 180, 3);
            timer->start();
            break;
         }

         case BLOCKED:
         {
            motorPair->drive(0, 0);
            servo->stop();
            servo->rotate(90);
            timer->stop();
            break;
         }
      }

      Behavior::setState(state);
   }

private:

   Timer* timer;

   MotorPair* motorPair;

   DistanceSensor* distanceSensor;

   ServoComponent* servo;
};

class ReverseBehavior : public Behavior, TimerListener
{

public:

   enum State
   {
      INIT,
      MOVING,
      DONE
   };

   ReverseBehavior(
      const String& id,
      MotorPair* motorPair) :
         Behavior(id)
   {
      this->motorPair = motorPair;

      setState(INIT);
   }

   ~ReverseBehavior() {}

   virtual void timeout(
      Timer* timer)
   {
      if (isEnabled())
      {
         if (getState() == MOVING)
         {
            setState(DONE);
         }
      }
   }

   virtual void setState(
      const int& state)
   {
      Logger::logDebug(F("ReverseBehavior::setState: %s -> %d"), getId().c_str(), state);

      switch (state)
      {
         case INIT:
         {
            motorPair->drive(0, 0);
            break;
         }

         case MOVING:
         {
            motorPair->drive(-100, 0);
            Timer* timer = Timer::newTimer("moveTimer", 1000, Timer::ONE_SHOT, this);
            timer->start();
            break;
         }

         case DONE:
         {
            motorPair->drive(0, 0);
            break;
         }
      }

      Behavior::setState(state);
   }

private:

   MotorPair* motorPair;
};

class RotateBehavior : public Behavior, TimerListener
{

public:

   enum State
   {
      INIT,
      ROTATING,
      DONE
   };

   RotateBehavior(
      const String& id,
      MotorPair* motorPair) :
         Behavior(id)
   {
      this->motorPair = motorPair;

      setState(INIT);
   }

   ~RotateBehavior() {}

   virtual void timeout(
      Timer* timer)
   {
      if (isEnabled())
      {
         if (getState() == ROTATING)
         {
            setState(DONE);
         }
      }
   }

   virtual void setState(
      const int& state)
   {
      Logger::logDebug(F("RotateBehavior::setState: %s -> %d"), getId().c_str(), state);

      switch (state)
      {
         case INIT:
         {
            motorPair->drive(0, 0);
            break;
         }

         case ROTATING:
         {
            motorPair->rotate(100);
            Timer* timer = Timer::newTimer("rotateTimer", 1000, Timer::ONE_SHOT, this);
            timer->start();
            break;
         }

         case DONE:
         {
            motorPair->drive(0, 0);
            break;
         }

         default:
         {
            break;
         }
      }

      Behavior::setState(state);
   }

private:

   MotorPair* motorPair;
};

}

// *****************************************************************************

ScoutBehavior::ScoutBehavior(
   const String& id,
   const String& motorPairId,
   const String& distanceSensorId,
   const String& servoId) :
      Behavior(id),
      motorPairId(motorPairId),
      distanceSensorId(distanceSensorId),
      servoId(servoId),
      initialized(false)
{
}

ScoutBehavior::ScoutBehavior(
   MessagePtr message) :
      Behavior(message->getString("id")),
      motorPairId(message->getString("motorPair")),
      distanceSensorId(message->getString("distanceSensor")),
      servoId(message->getString("servo")),
      initialized(false)
{
}

ScoutBehavior::~ScoutBehavior()
{
   delete (forwardBehavior);
   delete (reverseBehavior);
   delete (rotateBehavior);
}

void ScoutBehavior::setup()
{
   MotorPair* motorPair = static_cast<MotorPair*>(ToastBot::getComponent(motorPairId));
   DistanceSensor* distanceSensor = static_cast<DistanceSensor*>(ToastBot::getComponent(distanceSensorId));
   ServoComponent* servo = static_cast<ServoComponent*>(ToastBot::getComponent(servoId));

   if (motorPair && distanceSensor && servo)
   {
      initialized = true;

      forwardBehavior = new ForwardBehavior(getId() + ".forward", motorPair, distanceSensor, servo);
      reverseBehavior = new ReverseBehavior(getId() + ".reverse", motorPair);
      rotateBehavior = new RotateBehavior(getId() + ".rotate", motorPair);

      forwardBehavior->addListener(this);
      reverseBehavior->addListener(this);
      rotateBehavior->addListener(this);

      addChild(forwardBehavior);
      addChild(reverseBehavior);
      addChild(rotateBehavior);

      forwardBehavior->enable();
      reverseBehavior->enable();
      rotateBehavior->enable();

      disable();
   }
   else
   {
      Logger::logWarning(F("ScoutBehavior::setup: Failed to initialize all sub-components for ScoutBehavior [%s]"), getId().c_str());
   }
}

void ScoutBehavior::enable()
{
   if (initialized)
   {
      setState(FORWARD);
      Behavior::enable();
   }
}

void ScoutBehavior::disable()
{
   if (initialized)
   {
      setState(INIT);
      Behavior::disable();
   }
}

void ScoutBehavior::onStateChange(
   Behavior* behavior,
   const int& previousState,
   const int& newState)
{
   if (initialized && isEnabled())
   {
      if ((getState() == FORWARD) &&
          (behavior->getId() == (getId() + ".forward")) &&
          (newState == ForwardBehavior::BLOCKED))
      {
         setState(REVERSE);
      }
      else if ((getState() == REVERSE) &&
               (behavior->getId() == (getId() + ".reverse")) &&
               (newState == ReverseBehavior::DONE))
      {
         setState(ROTATE);
      }
      else if ((getState() == ROTATE) &&
               (behavior->getId() == (getId() + ".rotate")) &&
               (newState == RotateBehavior::DONE))
      {
         setState(FORWARD);
      }
   }
}

void ScoutBehavior::setState(
   const int& state)
{
   Logger::logDebug(F("ScoutBehavior::setState: %s -> %d"), getId().c_str(), state);

   if (initialized)
   {
      switch (state)
      {
         case INIT:
         {
            forwardBehavior->setState(ForwardBehavior::INIT);
            reverseBehavior->setState(ReverseBehavior::INIT);
            rotateBehavior->setState(RotateBehavior::INIT);
            break;
         }

         case FORWARD:
         {
            forwardBehavior->setState(ForwardBehavior::MOVING);
            break;
         }

         case REVERSE:
         {
            reverseBehavior->setState(ReverseBehavior::MOVING);
            break;
         }

         case ROTATE:
         {
            rotateBehavior->setState(RotateBehavior::ROTATING);
            break;
         }
      }

      Behavior::setState(state);
   }
}
