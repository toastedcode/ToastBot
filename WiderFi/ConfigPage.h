#pragma once

#include "ToastBot.h"

class ConfigPage : public Webpage
{

public:
  
  ConfigPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      Argument arguments[],
      int& numArguments,
      String& responsePath);

protected:

  virtual void replaceContent(
     String& content);
};
