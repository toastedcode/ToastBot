#pragma once

#include "Component.hpp"
#include "Messaging.hpp"
#include "Timer.hpp"

class WiderFi : public Component, TimerListener
{
  
public:
  
   // Constructor.
   inline WiderFi() :
      Component("WiderFi") {}

   // Destructor.
   inline virtual ~WiderFi() {}

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   void timeout(
      Timer* timer);

private:

   bool connectWifi(
      const String& ssid,
      const String& password,
      const int& connectionTimeout);

   bool startAccessPoint(
      const String& ssid,
      const String& password);

      
   bool isConnected() const;
      
   String getUniqueId() const;
};
