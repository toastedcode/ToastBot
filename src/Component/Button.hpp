// *****************************************************************************
// *****************************************************************************
// Button.hpp
//
// Author: Jason Tost
// Date:   6.28.2017
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Sensor.hpp"
#include "ComponentFactory.hpp"

class Button : public Sensor
{

public:

   // Constructor.
   Button(
      // A unique identifier for this component.
      const String& id,
      // The pin for the LED.
      const int& pin,
      // A flag indicating if the pin should be configured with INPUT (false) or INPUT_PULLUP (true).
      const bool& inputPullup = false);

   // Constructor.
   Button(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~Button();

   void setLongPress(
      const int& longPressTime);

   int getLongPress() const;

   virtual void setup();

   virtual void loop();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   virtual int read();

protected:

   virtual void onPoll();

   virtual void onButtonDown();

   virtual void onButtonUp();

   virtual void onButtonLongPress();

private:

   // This operation inteprets a pin reading, returning true if the reading indicates the button is down,
   // and false if it is up.
   bool interpret(
      const int& reading) const;

   // The GPIO pin to be read.
   int pin;

   // A flag indicating if the pin should be configured with INPUT (false) or INPUT_PULLUP (true).
   bool inputPullup;

   // The number of milliseconds the button must be held to be considered a "long" press.
   int longPressTime;

   // The current state of the button.
   int buttonState;

   // A system time (in millis) recording when the button was pressed.
   // Note: Used in detecting long presses.
   unsigned long downTime;
};

REGISTER(Button, Button)
