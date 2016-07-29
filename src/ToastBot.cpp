// *****************************************************************************
// *****************************************************************************
// ToastBot.cpp
//
// Author: Jason Tost
// Date:   7.12.2016
//
// *****************************************************************************
// *****************************************************************************

#include "BasicMessage.hpp"
#include "Messaging.hpp"
#include "ToastBot.hpp"

const int ToastBot::MAX_COMPONENTS;

Set<Component*, ToastBot::MAX_COMPONENTS> ToastBot::components;

bool ToastBot::add(
   Component* component)
{
   bool isSuccess = false;

   if (components.add(component) == true)
   {
      MessageRouter::registerHandler(component);
      isSuccess = true;
   }

   return (isSuccess);
}

bool ToastBot::remove(
   Component* component)
{
   bool isSuccess = false;

   if (components.remove(component) == true)
   {
      MessageRouter::unregisterHandler(component);
      isSuccess = true;
   }

   return (isSuccess);
}

void ToastBot::setup(
   const String& id)
{
   Messaging::setup<BasicMessage>(id, 50);

   for (int i = 0; i < components.length(); i++)
   {
      components.item(i)->value->setup();
   }
}

void ToastBot::loop()
{
   for (int i = 0; i < components.length(); i++)
   {
      components.item(i)->value->loop();
   }
}
