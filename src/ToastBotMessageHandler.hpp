// *****************************************************************************
// *****************************************************************************
// ToastBotMessageHandler.hpp
//
// Author: Jason Tost
// Date:   1.20.2019
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Component.hpp"

class ToastBotMessageHandler : public Component
{

public:

   // Constructor.
   ToastBotMessageHandler();

   // Destructor.
   virtual ~ToastBotMessageHandler();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

protected:

   virtual MessagePtr pingReply(
      MessagePtr ping);
};
