#pragma once


#include "Component.hpp"
#include "BehaviorListener.hpp"
#include "ListSet.hpp"

class Behavior : public Component
{

public:

   Behavior(
      const String& id);

   virtual ~Behavior();

   void handleMessage(
      MessagePtr message);

   virtual void setup();

   virtual void loop();

   int getState() const;

   virtual void setState(
      const int& state);

   virtual void enable();

   virtual void disable();

   bool isEnabled() const;

   void addChild(
      Behavior* child);

   void removeChild(
      Behavior* child);

   void addListener(
      BehaviorListener* child);

   void removeListener(
      BehaviorListener* child);

private:

   bool enabled;

   int state;

   ListSet<Behavior*> children;

   ListSet<BehaviorListener*> listeners;
};

inline Behavior::Behavior(
   const String& id) :
      Component(id),
      state(0),
      enabled(false)
{
}

inline Behavior::~Behavior()
{
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

inline void Behavior::addChild(
   Behavior* child)
{
   children.insert(child);
}

inline void Behavior::removeChild(
   Behavior* child)
{
   children.erase(child);
}

inline void Behavior::addListener(
   BehaviorListener* listener)
{
   listeners.insert(listener);
}

inline void Behavior::removeListener(
   BehaviorListener* listener)
{
   listeners.erase(listener);
}
