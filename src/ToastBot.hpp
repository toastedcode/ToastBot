// *****************************************************************************
// *****************************************************************************
// ToastBot.hpp
//
// Author: Jason Tost
// Date:   7.12.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"
#include "Set.hpp"

class ToastBot
{

public:

   static const int MAX_COMPONENTS = 10;

   static void setId(
      const String& id);

   static String getId();

   static bool add(
      Component* component);

   static bool remove(
      Component* component);

   static void setup(
      const String& id);

   static void loop();

private:

   static String id;

   static Set<Component*, MAX_COMPONENTS> components;
};

inline void ToastBot::setId(
   const String& id)
{
   ToastBot::id = id;
}

inline String ToastBot::getId()
{
   return (id);
}
