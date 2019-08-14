#pragma once

#include "ClientAdapter.hpp"
#include "ComponentFactory.hpp"
#include "EspWifi.hpp"

class TcpClientAdapter : public ClientAdapter
{

public:

   TcpClientAdapter(
      const String& id,
      Protocol* protocol,
      const String& host,
      const int& port,
      const int& retryDelay);

   TcpClientAdapter(
      MessagePtr parameters);

   // **************************************************************************
   //                            Component interface

   virtual void setup();

   virtual void loop();

   // **************************************************************************
   //                            Adapter interface

   virtual bool sendRemoteMessage(
      MessagePtr message);

   virtual MessagePtr getRemoteMessage();

   // **************************************************************************
   //                          ClientAdapter interface

   virtual bool connect();

   virtual bool disconnect();

   virtual bool isConnected();

   // **************************************************************************

private:

   static const int BUFFER_SIZE = 256;

   String host;

   int port;

   WiFiClient client;

   long retryTime;

   int retryDelay;  // milliseconds

   char buffer[BUFFER_SIZE];

   int readIndex = 0;
};

REGISTER(TcpClientAdapter, TcpClientAdapter)

