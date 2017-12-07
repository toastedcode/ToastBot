#include "FollowAI_2.hpp"

const int ADJACENT_DISTANCE = 20;  // cm
const int NEAR_DISTANCE = 50;  // cm
const int FAR_DISTANCE = 100;  // cm

const int SEARCH_SPEED = ((MotorPair::MAX_SPEED * 80) / 100);

const int SEARCH_YAW = 50;

const int FOLLOW_SPEEDS[] =
{
   0,                                    // NONE
   0,                                    // ADJACENT,
   ((MotorPair::MAX_SPEED * 80) / 100),  // NEAR,
   MotorPair::MAX_SPEED,                 // FAR
};

const bool OFF = false;
const bool ON = true;

const char* READING_TIMER_ID = "reading";
const char* WAITING_TIMER_ID = "waiting";
const char* SEARCH_TIMER_ID = "search";
const char* LOST_TIMER_ID = "lost";

const int READING_TIMER_PERIOD = 500;  // milliseconds
const int WAITING_TIMER_PERIOD = 15000;  // milliseconds
const int SEARCH_TIMER_PERIOD = 1000;  // milliseconds
const int LOST_TIMER_PERIOD = 5000;  // milliseconds

FollowAI_2::FollowAI_2(
   const String& id,
   DistanceSensor* sensor,
   MotorPair* motorPair) : Component(id),
                           sensor(sensor),
                           motorPair(motorPair),
                           isAiEnabled(false),
                           readingTimer(0),
                           waitingTimer(0),
                           lostTimer(0),
                           searchTimer(0),
                           state(WAITING),
                           searchDirection(LEFT),
                           proximity(NONE)
{
}

FollowAI_2::~FollowAI_2()
{
   Timer::freeTimer(readingTimer);
   Timer::freeTimer(waitingTimer);
   Timer::freeTimer(lostTimer);
   Timer::freeTimer(searchTimer);
}

void FollowAI_2::setup()
{
}

void FollowAI_2::handleMessage(
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

void FollowAI_2::timeout(
   Timer* timer)
{
   if (isEnabled())
   {
      if (timer->getId() == READING_TIMER_ID)
      {
         onReadingTimeout();
      }
      else if (timer->getId() == WAITING_TIMER_ID)
      {
         onWaitingTimeout();
      }
      else if (timer->getId() == LOST_TIMER_ID)
      {
         onLostTimeout();
      }
      else if (timer->getId() == SEARCH_TIMER_ID)
      {
         onSearchTimeout();
      }
      else
      {
         Timer::freeTimer(timer);
      }
   }
}

void FollowAI_2::enable()
{
   isAiEnabled = true;

   setReadingTimer(ON);

   setState(WAITING);
}

void FollowAI_2::disable()
{
   isAiEnabled = false;

   setReadingTimer(OFF);
   setWaitingTimer(OFF);
   setLostTimer(OFF);
   setSearchTimer(OFF);

   motorPair->drive(0, 0);

   state = WAITING;
}

void FollowAI_2::setState(
   const State& state)
{
   if (this->state != state)
   {
      this->state = state;

      Logger::logDebug(F("FollowAI_2::setState: state = %d"), state);

      switch (this->state)
      {
         case WAITING:
         {
            setWaitingTimer(ON);
            setLostTimer(OFF);
            setSearchTimer(OFF);
            break;
         }

         case FOLLOWING:
         {
            setWaitingTimer(OFF);
            setLostTimer(OFF);
            setSearchTimer(OFF);
            break;
         }

         case LOST:
         {
            setWaitingTimer(OFF);
            setLostTimer(ON);
            setSearchTimer(ON);
            break;
         }

         default:
         {
            break;
         }
      }
   }
}

void FollowAI_2::onProximity(
   const Proximity& proximity)
{
   if (state == FOLLOWING)
   {
      switch (proximity)
      {
         case NONE:
         {
            setState(LOST);

            searchDirection = LEFT;
            motorPair->drive(SEARCH_SPEED, getYaw(searchDirection));
            break;
         }

         case ADJACENT:
         case NEAR:
         case FAR:
         {
            motorPair->drive(getSpeed(proximity), 0);
            break;
         }
      }
   }
   else if (proximity != NONE)
   {
      setState(FOLLOWING);

      motorPair->drive(getSpeed(proximity), 0);
   }
}

void FollowAI_2::onReadingTimeout()
{
   int reading = DistanceSensor::toCentimeters(sensor->read());

   Proximity newProximity = getProximity(reading);

   if (newProximity != proximity)
   {
      proximity = newProximity;

      Logger::logDebug(F("FollowAI_2::onReadingTimeout: proximity = %d"), proximity);

      onProximity(proximity);
   }
}

void FollowAI_2::onWaitingTimeout()
{
   Logger::logDebug(F("FollowAI_2::onWaitingTimeout"));

   if (state == WAITING)
   {
      // TODO: Implement periodic rotation.
   }
}

void FollowAI_2::onLostTimeout()
{
   Logger::logDebug(F("FollowAI_2::onLostTimeout"));

   if (state == LOST)
   {
      setState(WAITING);
      motorPair->drive(0, 0);
   }
}

void FollowAI_2::onSearchTimeout()
{
   if (state == LOST)
   {
      searchDirection = (searchDirection == LEFT) ? RIGHT : LEFT;

      Logger::logDebug(F("FollowAI_2::onSearchTimeout: direction = %d"), searchDirection);

      motorPair->drive(SEARCH_SPEED, getYaw(searchDirection));
   }
}

void FollowAI_2::setReadingTimer(
   const bool& isOn)
{
   if (!readingTimer)
   {
      readingTimer = Timer::newTimer(READING_TIMER_ID, READING_TIMER_PERIOD, Timer::PERIODIC, this);
   }

   if (isOn && !readingTimer->isStarted())
   {
      readingTimer->start();
   }
   else if (!isOn && readingTimer->isStarted())
   {
      readingTimer->stop();
   }
}

void FollowAI_2::setWaitingTimer(
   const bool& isOn)
{
   if (!waitingTimer)
   {
      waitingTimer = Timer::newTimer(WAITING_TIMER_ID, WAITING_TIMER_PERIOD, Timer::PERIODIC, this);
   }

   if (isOn && !waitingTimer->isStarted())
   {
      waitingTimer->start();
   }
   else if (!isOn && waitingTimer->isStarted())
   {
      waitingTimer->stop();
   }
}

void FollowAI_2::setLostTimer(
   const bool& isOn)
{
   if (!lostTimer)
   {
      lostTimer = Timer::newTimer(LOST_TIMER_ID, LOST_TIMER_PERIOD, Timer::PERIODIC, this);
   }

   if (isOn && !lostTimer->isStarted())
   {
      lostTimer->start();
   }
   else if (!isOn && lostTimer->isStarted())
   {
      lostTimer->stop();
   }
}

void FollowAI_2::setSearchTimer(
   const bool& isOn)
{
   if (!searchTimer)
   {
      searchTimer = Timer::newTimer(SEARCH_TIMER_ID, SEARCH_TIMER_PERIOD, Timer::PERIODIC, this);
   }

   if (isOn && !searchTimer->isStarted())
   {
      searchTimer->start();
   }
   else if (!isOn && searchTimer->isStarted())
   {
      searchTimer->stop();
   }
}

FollowAI_2::Proximity FollowAI_2::getProximity(
   const int& reading)
{
   Proximity proximity = NONE;

   if (reading == 0)
   {
      proximity = NONE;
   }
   else if (reading <= ADJACENT_DISTANCE)
   {
      proximity = ADJACENT;
   }
   else if (reading < NEAR_DISTANCE)
   {
      proximity = NEAR;
   }
   else if (reading < FAR_DISTANCE)
   {
      proximity = FAR;
   }

   return (proximity);
}

int FollowAI_2::getSpeed(
   const Proximity& proximity)
{
   return (FOLLOW_SPEEDS[proximity]);
}

int FollowAI_2::getYaw(
   const SearchDirection& searchDirection)
{
   return ((searchDirection == LEFT) ? SEARCH_YAW : (SEARCH_YAW * -1));
}

