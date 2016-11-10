#include "Logger.hpp"
#include "ScoutBehavior.hpp"

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
      DistanceSensor* distanceSensor) :
         Behavior(id)
   {
      this->motorPair = motorPair;
      this->distanceSensor = distanceSensor;

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
      if (isEnabled())
      {
         int reading = DistanceSensor::toCentimeters(distanceSensor->read());

         if ((getState() == MOVING) &&
             (reading <= 10))
         {
            setState(BLOCKED);
         }
      }
   }

   virtual void setState(
      const int& state)
   {
      switch (state)
      {
         case INIT:
         {
            motorPair->drive(0, 0);
            timer->stop();
            break;
         }

         case MOVING:
         {
            motorPair->drive(100, 0);
            timer->start();
            break;
         }

         case BLOCKED:
         {
            motorPair->drive(0, 0);
            timer->stop();
            break;
         }
      }

      Behavior::setState(state);

      Logger::logDebug("%s: -> %d", getId().c_str(), state);
   }

private:

   Timer* timer;

   MotorPair* motorPair;

   DistanceSensor* distanceSensor;
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
            Timer* timer = Timer::newTimer("moveTimer", 3000, Timer::ONE_SHOT, this);
            timer->start();
            break;
         }

         case DONE:
         {
            motorPair->drive(0, 0);
            break;
         }
      }

      Logger::logDebug("%s: -> %d", getId().c_str(), state);

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
            Timer* timer = Timer::newTimer("rotateTimer", 3000, Timer::ONE_SHOT, this);
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

      Logger::logDebug("%s: -> %d", getId().c_str(), state);

      Behavior::setState(state);
   }

private:

   MotorPair* motorPair;
};

}

// *****************************************************************************

ScoutBehavior::ScoutBehavior(
   const String& id,
   MotorPair* motorPair,
   DistanceSensor* distanceSensor) :
      Behavior(id)
{
   forwardBehavior = new ForwardBehavior(id + ".forward", motorPair, distanceSensor);
   reverseBehavior = new ReverseBehavior(id + ".reverse", motorPair);
   rotateBehavior = new RotateBehavior(id + ".rotate", motorPair);

   forwardBehavior->addListener(this);
   reverseBehavior->addListener(this);
   rotateBehavior->addListener(this);

   addChild(forwardBehavior);
   addChild(reverseBehavior);
   addChild(rotateBehavior);

   disable();
}

ScoutBehavior::~ScoutBehavior()
{
   delete (forwardBehavior);
   delete (reverseBehavior);
   delete (rotateBehavior);
}

void ScoutBehavior::handleMessage(
   MessagePtr message)
{
   // enable
   if (message->getMessageId() == "enable")
   {
      enable();
      message->setFree();
   }
   // disable
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

void ScoutBehavior::enable()
{
   setState(FORWARD);
   Behavior::enable();
}

void ScoutBehavior::disable()
{
   setState(INIT);
   Behavior::disable();
}

void ScoutBehavior::onStateChange(
   Behavior* behavior,
   const int& previousState,
   const int& newState)
{
   if (isEnabled())
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

   Logger::logDebug("%s: -> %d", getId().c_str(), state);

   Behavior::setState(state);
}
