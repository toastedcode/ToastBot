// *****************************************************************************
// *****************************************************************************
// RemoteLogger.cpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Messaging.hpp"
#include "RemoteLogger.hpp"

void RemoteLogger::log(
   const LogLevel& logLevel,
   const String& string)
{
   if (adapterId.length() != 0)
   {
      Message* message = Messaging::newMessage();

      message->setMessageId("logMessage");
      message->set("logLevel", toString(logLevel));
      message->set("message", string);

      message->setDestination(adapterId);

      Messaging::send(message);
   }
}
