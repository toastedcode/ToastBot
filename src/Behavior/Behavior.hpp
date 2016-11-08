#pragma once


#include "Component.hpp"
#include "BehaviorListener.hpp"
#include "Set.hpp"

class Behavior : public Component
{

public:

   Behavior(
      const String& id);

   virtual ~Behavior();

   virtual void setup();

   virtual void loop();

   virtual void handleMessage(
      Message* message);

   int getState() const;

   virtual void setState(
      const int& state);

   virtual void enable();

   virtual void disable();

   bool isEnabled() const;

   bool addChild(
      Behavior* child);

   bool removeChild(
      Behavior* child);

   bool addListener(
      BehaviorListener* child);

   bool removeListener(
      BehaviorListener* child);

private:

   bool enabled;

   int state;

   Set<Behavior*, 5> children;

   Set<BehaviorListener*, 5> listeners;
};

inline Behavior::Behavior(
   const String& id) :
      Component(id)
{
}

inline Behavior::~Behavior()
{
}

inline void Behavior::handleMessage(
   Message* message)
{
   Component::handleMessage(message);
}

inline int Behavior::getState() const
{
   return (state);
}

inline void Behavior::enable()
{
   enabled = true;
}

inline void Behavior::disable()
{
   enabled = false;
}

inline bool Behavior::isEnabled() const
{
   return (enabled);
}

inline bool Behavior::addChild(
   Behavior* child)
{
   return (children.add(child));
}

inline bool Behavior::removeChild(
   Behavior* child)
{
   return (children.remove(child));
}

inline bool Behavior::addListener(
   BehaviorListener* listener)
{
   return (listeners.add(listener));
}

inline bool Behavior::removeListener(
   BehaviorListener* listener)
{
   return (listeners.remove(listener));
}
