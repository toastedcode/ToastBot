#pragma once

#include "ToastBot.h"

class FactoryResetButton : public Button
{

public:  

   // Constructor.
   FactoryResetButton(
      // A unique identifier for this component.
      const String& id,
      // The pin for the LED.
      const int& pin,
      // A flag indicating if the pin should be configured with INPUT (false) or INPUT_PULLUP (true).
      const bool& inputPullup = false);

   // Constructor.
   FactoryResetButton(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);
      
   virtual void onButtonLongPress();
};

inline FactoryResetButton::FactoryResetButton(
  const String& id,
  const int& pin,
  const bool& inputPullup) :
   Button(id, pin, inputPullup)
{
  // Nothing to do here.    
}

inline FactoryResetButton::FactoryResetButton(
  MessagePtr message) :
   Button(message)
{
   // Nothing to do here.  
}
      
inline void FactoryResetButton::onButtonLongPress()
{
   ToastBot::factoryReset();
}
