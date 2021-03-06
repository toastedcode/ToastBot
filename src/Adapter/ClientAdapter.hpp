#pragma once

#include "Adapter.hpp"

class ClientAdapter : public Adapter
{

public:

   ClientAdapter(
      const String& id,
      Protocol* protocol);

   ClientAdapter(
      MessagePtr parameters);

   virtual bool connect() = 0;

   virtual bool disconnect() = 0;

   virtual bool isConnected() = 0;
};

inline ClientAdapter::ClientAdapter(
   const String& id,
   Protocol* protocol) :
      Adapter(id, protocol)
{
   // Nothing to do here.
}

inline ClientAdapter::ClientAdapter(
   MessagePtr parameters) :
      Adapter(parameters)
{
   // Nothing to do here.
}
