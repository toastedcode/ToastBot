#pragma once

#include <ESP8266HTTPClient.h>

#include "Adapter.hpp"
#include "ComponentFactory.hpp"
#include "../Messaging/RestfulProtocol.hpp"

class HttpClientAdapter : public Adapter
{

public:

   HttpClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& defaultUrl = "");

   HttpClientAdapter(
      MessagePtr parameters);

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

private:

   HTTPClient http;

   RestfulProtocol sendProtocol;

   String defaultUrl;
};

REGISTER(HttpClientAdapter, HttpClientAdapter)
