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

   for (int i = 0; i < children.length(); i++)
   {
      children.item(i)->value->setup();
   }
}

void Behavior::loop()
{
   Component::loop();

   if (isEnabled())
   {
      for (int i = 0; i < children.length(); i++)
      {
         children.item(i)->value->loop();
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

      for (int i = 0; i < listeners.length(); i++)
      {
         listeners.item(i)->value->onStateChange(this, previousState, state);
      }
   }
}
