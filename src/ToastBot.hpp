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

#include "Board.hpp"
#include "Component.hpp"
#include "Properties.hpp"
#include "Set.hpp"

class ToastBot
{

public:

   static const int MAX_COMPONENTS = 30;

   static void setId(
      const String& id);

   static String getId();

   static bool add(
      Component* component,
      const bool& setDefaultHandler = false);

   static bool remove(
      Component* component);

   static Component* get(
      const String& id);

   static const Properties& getProperties();

   template<typename T>
   inline static const void setProperty(
      const String& name,
      const T& value)
   {
      properties.set(name, value);
      properties.save();
   }

   static void setup(
      Board* board);

   static void loop();

private:

   static String id;

   static Properties properties;

   static Set<Component*, MAX_COMPONENTS> components;

   static bool initialized;
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
