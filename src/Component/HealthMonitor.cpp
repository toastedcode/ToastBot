#include "HealthMonitor.hpp"
#include "Logger.hpp"

HealthMonitor::HealthMonitor(
   const String& id,
   const String& adapterId,
   const int& pingRate,
   const int& failureThreshold) :
      Component(id),
      adapterId(adapterId),
      health(HEALTH_UNKNOWN),
      pingRate(pingRate),
      failureThreshold(failureThreshold),
      gotReply(false),
      missedPings(0),
      receivedPings(0),
      timer(0)
{
   timer = Timer::newTimer(
      getId() + ".timer",
      pingRate,
      Timer::PERIODIC,
      this);

   timer->setListener(this);
}

HealthMonitor::~HealthMonitor()
{
   Timer::freeTimer(timer);
}

void HealthMonitor::setHealth(
   const Health& health)
{
   Health previousHealth = health;
   this->health = health;

   if (adapterId.length() > 0)
   {
      Logger::logDebug("HealthMonitor::setHealth: Adapter [%s]: %s -> %s",
                       adapterId.c_str(),
                       toString(previousHealth).c_str(),
                       toString(health).c_str());

      MessagePtr message = Messaging::newMessage();
      message->setTopic("health");
      message->setSource(adapterId);
      message->set("health", toString(health));
      Messaging::publish(message);
   }
}

Health HealthMonitor::getHealth() const
{
   return (health);
}

void HealthMonitor::start()
{
   health = HEALTH_UNKNOWN;
   missedPings = 0;
   gotReply = false;

   timer->start();
}

void HealthMonitor::stop()
{
   health = HEALTH_UNKNOWN;
   missedPings = 0;
   gotReply = false;

   timer->stop();
}

void HealthMonitor::handleMessage(
   MessagePtr message)
{
   if (message->getMessageId() == "pong")
   {
      gotReply = true;
      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}

void HealthMonitor::timeout(
   Timer* timer)
{
   if (!gotReply)
   {
      onUnsuccessfulPing();
   }
   else
   {
      onSuccessfulPing();
   }

   // Next ping.
   MessagePtr message = Messaging::newMessage();
   message->setMessageId("ping");
   message->setSource(getId());
   message->setDestination(adapterId);
   Messaging::send(message);

   gotReply = false;
}

void HealthMonitor::onSuccessfulPing()
{
   switch (health)
   {
      case RECOVERING:
      {
         receivedPings++;

         if (receivedPings > failureThreshold)
         {
            setHealth(HEALTHY);
         }
         break;
      }

      case HEALTH_UNKNOWN:
      case FAILING:
      case FAILED:
      {
         missedPings = 0;
         receivedPings++;
         setHealth(RECOVERING);
         break;
      }

      case HEALTHY:
      default:
      {
         // Nothing to do.
         break;
      }
   }
}


void HealthMonitor::onUnsuccessfulPing()
{
   switch (health)
   {
      case HEALTHY:
      case RECOVERING:
      {
         receivedPings = 0;
         missedPings++;
         setHealth(FAILING);
         break;
      }

      case FAILING:
      {
         missedPings++;

         if (missedPings > failureThreshold)
         {
            setHealth(FAILED);
         }
         break;
      }

      case HEALTH_UNKNOWN:
      case FAILED:
      default:
      {
         // Nothing to do.
         break;
      }
   }
}
