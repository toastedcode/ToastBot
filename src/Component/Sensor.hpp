// *****************************************************************************
// *****************************************************************************
// Sensor.hpp
//
// Author: Jason Tost
// Date:   10.20.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class Sensor : public Component, public TimerListener
{

public:

   // Constructor.
   Sensor(
      // A unique identifier for this sensor.
      const String& id);

   // Constructor.
   Sensor(
      // A message containing the parameters to use in creating the component.
      MessagePtr message);

   // Destructor.
   virtual ~Sensor();

   // **************************************************************************
   //                         MessageHandler interface

   // This operation handles a message directed to this component.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // **************************************************************************
   //                         TimerListner interface

   virtual void timeout(
      Timer* timer);

   // **************************************************************************

   virtual int read() = 0;

   // This operation sets up automatic polling on the sensor.
   virtual void poll(
      // The rate at which updates should be sent, in milliseconds.
      const int& period);

protected:

   virtual void onPoll();

   virtual void sendReading(
         const String& destination);

   virtual void broadcastReading();

private:

   Timer* pollTimer;
};
