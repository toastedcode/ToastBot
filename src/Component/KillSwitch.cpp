#include "KillSwitch.hpp"

static const int KEEP_ALIVE_PERIOD = 1000;

KillSwitch::KillSwitch(
   const String& id) :
      Component(id),
      timer(0)
{
}

KillSwitch::~KillSwitch()
{
}

void KillSwitch::reset()
{
   timer->reset();
}

void KillSwitch::setup()
{
   timer = Timer::newTimer(
      getId() + ".timer",
      KEEP_ALIVE_PERIOD,
      Timer::PERIODIC,
      this);

   timer->setListener(this);

   timer->start();
}

void KillSwitch::timeout(
   Timer* timer)
{
}
