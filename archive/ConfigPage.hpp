#pragma once

#include "Messaging.h"
#include "Webpage.hpp"

class ConfigPage : public Webpage
{

public:
  
  ConfigPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      MessagePtr message,
      String& responsePath);

protected:

  virtual void replaceContent(
     String& content);
};
