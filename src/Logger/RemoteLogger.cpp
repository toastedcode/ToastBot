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
      if (message)
      {
         message->setMessageId("logMessage");
         message->set("logLevel", toString(logLevel));
         message->set("message", string);

         message->setDestination(adapterId);

         Messaging::send(message);

#ifdef ARDUINO
         printf_P((PGM_P)F("Remotely logged: %s: %s\n"), toString(logLevel).c_str(), string.c_str());
#else
         printf("Remotely logged: %s: %s\n", toString(logLevel).c_str(), string.c_str());
#endif
      }
      else
      {
#ifdef ARDUINO
         printf_P((PGM_P)F("Failed to remotely log: %s: %s\n"), toString(logLevel).c_str(), string.c_str());
#else
         printf("Failed to remotely log: %s: %s\n", toString(logLevel).c_str(), string.c_str());
#endif
      }
   }
}
