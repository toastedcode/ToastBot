#pragma once

#include "TcpServerAdapter.hpp"

class HttpServerAdapter : public TcpServerAdapter
{

public:

   HttpServerAdapter(
      const String& id);

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   String getMessageFromHttpRequest(
      const String& httpRequestString);

};
