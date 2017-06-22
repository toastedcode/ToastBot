#pragma once

#include "Component.hpp"
#include "Map.hpp"
#include "Messaging.hpp"

// *****************************************************************************
//                               Typedefs
// *****************************************************************************

static const int MAX_REGISTERED_COMPONENT_TYPES = 20;

typedef Component* (*ComponentConstructor)(MessagePtr);

typedef Map<String, ComponentConstructor, MAX_REGISTERED_COMPONENT_TYPES> ConstructorRegistry;

// *****************************************************************************
//                           ComponentFactory
// *****************************************************************************

class ComponentFactory
{

public:

   static void registerComponent(
      const String& classId,
      ComponentConstructor constructor);

   static Component* create(
      const String& classId,
      MessagePtr message);

   static ConstructorRegistry registry;
};

// *****************************************************************************
//                           ComponentCreator
// *****************************************************************************

template <class T>
class ComponentCreator
{

public:

   ComponentCreator(
      const String& classId)
   {
      ComponentFactory::registerComponent(classId, create);
   }

   static Component* create(
      MessagePtr message)
   {
      return (new T(message));
   }
};
