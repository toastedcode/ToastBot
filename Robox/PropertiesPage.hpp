#pragma once

#include "Webpage.hpp"

class PropertiesPage : public Webpage
{

public:
  
   PropertiesPage();

  virtual ~PropertiesPage();

  virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      const Dictionary& arguments,
      String& responsePath);

protected:

  void getPropertyDiv(
     const String& propertyName,
     const String& propertyValue,
     String& content);
};
