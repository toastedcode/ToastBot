// *****************************************************************************
// *****************************************************************************
// Led.hpp
//
// Author: Jason Tost
// Date:   6.20.2017
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"

class Led : public Component
{

public:

   // Constructor.
   Led(
      // A unique identifier for this component.
      const String& id,
      // The pin for the LED.
      const int& pin);

   // Constructor.
   Led(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~Led();

   void setBrightness(
      const int& brightness,
      const bool& stopPattern = true);

   void blink(
      const String& patternString);

   void pulse(
      const int& period);

   // This operation should be called on startup to prepare the sensor for polling/updating.
   virtual void setup();

   virtual void loop();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   static const int MIN_BRIGHTNESS = 0;

   static const int MAX_BRIGHTNESS = 100;

   static const int MIN_PWM = 0;

   static const int MAX_PWM = 1023;

private:

   class LedPattern
   {

   public:

      LedPattern(
         Led* led,
         String patternString);

      ~LedPattern();

      void setPattern(
         const String& pattern);

      void update();

   private:

      int getNext();

      void parse(String patternString);

      static const int MAX_PATTERN_LENGTH = 20;

      static const int PATTERN_UPDATE_PERIOD;  // milliseconds

      Led* led;

      int patternArray[MAX_PATTERN_LENGTH];

      int patternArraySize;

      int patternIndex;

      long updateTime;
   };

   class LedPulse
   {

   public:

      LedPulse(
         Led* led,
         const int& period);

      ~LedPulse();

      void setPeriod(
         const int& period);

      void update();

   private:

      Led* led;

      int period;
   };

   char nextToken();

   // The GPIO pin used for controlling the angle of the servo.
   int pin;

   // The brightness of the LED, represented as a percentage between 0% adn 100%.
   int brightness;

   // A pattern of blinks, created with a String where
   //   "_" is 250 milliseconds of 0% brightness
   //   "-" is 250 milliseconds of 100% brightness
   //   "/" is a 250 millisecond linear increase in brightness
   //   "\" is a 250 millisecond linear decrease in brightness
   //
   // Example patterns:
   // Steady blink: ____----
   // Fade-in/out:  ////////\\\\\\\\
   // SOS:          --__--__--__--------__--------__--------__--__--__--____
   LedPattern* ledPattern;

   LedPulse* ledPulse;
};
