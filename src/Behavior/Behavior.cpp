#include "Behavior.hpp"
#include "Logger.hpp"

void Behavior::handleMessage(
   MessagePtr message)
{
   // enable
   if (message->getMessageId() == "enable")
   {
      Logger::logDebug(F("Behavior::handleMessage: %s.enable()"), getId().c_str());

      enable();
      message->setFree();
   }
   // disable
   else if (message->getMessageId() == "disable")
   {
      Logger::logDebug(F("Behavior::handleMessage: %s.disable()"), getId().c_str());

      disable();
      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}

void Behavior::setup()
{
   Component::setup();

   for (Set<Behavior*>::Iterator it = children.begin(); it != children.end(); it++)
   {
      (*it)->setup();
   }
}

void Behavior::loop()
{
   Component::loop();

   if (isEnabled())
   {
      for (Set<Behavior*>::Iterator it = children.begin(); it != children.end(); it++)
      {
         (*it)->loop();
      }
   }
}

void Behavior::setState(
   const int& state)
{
   if (this->state != state)
   {
      int previousState = this->state;
      this->state = state;

      for (Set<BehaviorListener*>::Iterator it = listeners.begin(); it != listeners.end(); it++)
      {
         (*it)->onStateChange(this, previousState, state);
      }
   }
}
