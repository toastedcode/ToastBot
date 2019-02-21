// *****************************************************************************
// *****************************************************************************
// RemoteLogger.hpp
//
// Author: Jason Tost
// Date:   10.10.2016
//
// *****************************************************************************
// *****************************************************************************

#pragma once

#include "Adapter.hpp"
#include "Logger.hpp"

class RemoteLogger : public Logger
{

public:

   RemoteLogger(
      const String& target);

   virtual ~RemoteLogger();

   void setTarget(
      const String& target);

   virtual void log(
      const LogLevel& logLevel,
      const String& logMessage);

protected:

   String target;
};

inline RemoteLogger::RemoteLogger(
   const String& target) :
      Logger(),
      target(target)
{
}

inline RemoteLogger::~RemoteLogger()
{
}

inline void RemoteLogger::setTarget(
   const String& adapterId)
{
   this->target = target;
}
