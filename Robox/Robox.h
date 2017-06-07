#pragma once

#include "Component.hpp"
#include "Messaging.hpp"

class Robox : public Component
{
  
public:
  
   // Constructor.
   inline Robox() :
      Component("robox") {}

   // Destructor.
   inline virtual ~Robox() {}

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // Creates a unique id using the MAC address.
   // Useful for creating AP names.
   static String getUniqueId();
};
