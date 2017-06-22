#include "ComponentFactory.hpp"
#include "Logger.hpp"

ConstructorRegistry ComponentFactory::registry;

void ComponentFactory::registerComponent(
   const String& name,
   ComponentConstructor constructor)
{
    // Register the class constructor.
   registry[name] = constructor;

   Logger::logDebug("ComponentFactory::registerComponent: Registered component [%s].", name.c_str());
}

Component* ComponentFactory::create(
   const String& classId,
   MessagePtr message)
{
   Component* component = 0;

   const ComponentConstructor* constructor = registry.get(classId);

   if (constructor)
   {
      component = (*constructor)(message);

      Logger::logDebug("ComponentFactory::create: Created new %s component.", classId.c_str());
   }
   else
   {
      Logger::logWarning("ComponentFactory::create: No component registered under class id [%s].", classId.c_str());
   }

   return (component);
}
