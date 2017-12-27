#include <ESP8266WebServer.h>

#include "CommonDefs.hpp"
#include "Messaging.hpp"

class Webpage
{

public:

   Webpage(
      const String& uri);

   Webpage(
      const String& uri,
      const String& path);

   virtual ~Webpage();

   String getUri() const;

   String getPath() const;

   virtual bool canHandle(
      const HTTPMethod& requestMethod,
      const String& requestUri) const;

   virtual bool handle(
      const HTTPMethod& requestMethod,
      const String& requestUri,
      MessagePtr message,
      String& responsePath);

protected:

   virtual void replaceContent(
      String& content);

private:

   String uri;

   String path;
};

