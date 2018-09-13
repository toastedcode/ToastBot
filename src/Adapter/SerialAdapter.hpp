#pragma once

#include "ComponentFactory.hpp"
#include "Adapter.hpp"

class SerialAdapter : public Adapter
{

public:

   SerialAdapter(
      const String& id,
      Protocol* protocol);

   SerialAdapter(
      MessagePtr parameters);

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();
};


REGISTER(SerialAdapter, SerialAdapter)
