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

   static String getId();

   static bool addComponent(
      Component* component,
      const bool& setDefaultHandler = false);

   static bool removeComponent(
      Component* component);

   static Component* getComponent(
      const String& id);

   static Properties& getProperties();

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

   static void factoryReset();

private:

   static String getUniqueId();

   static Properties properties;

   static Set<Component*, MAX_COMPONENTS> components;

   static bool initialized;
};
