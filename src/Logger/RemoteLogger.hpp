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
      const String& adapterId);

   virtual ~RemoteLogger();

   void setAdapter(
      const String& adapterId);

   virtual void log(
      const LogLevel& logLevel,
      const String& string);

protected:

   String adapterId;
};

inline RemoteLogger::RemoteLogger(
   const String& adapterId) :
      Logger(),
      adapterId(adapterId)
{
}

inline RemoteLogger::~RemoteLogger()
{
}

inline void RemoteLogger::setAdapter(
   const String& adapterId)
{
   this->adapterId = adapterId;
}
