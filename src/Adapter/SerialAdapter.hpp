#pragma once

#include "Adapter.hpp"

class SerialAdapter : public Adapter
{

public:

   SerialAdapter(
      const String& id,
      Protocol* protocol);

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();
};
