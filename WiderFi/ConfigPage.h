#pragma once

#include "ToastBot.h"

class ConfigPage : public Webpage
{

public:
  
  ConfigPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      const Dictionary&, arguments,
      String& responsePath);

protected:

  virtual void replaceContent(
     String& content);
};
