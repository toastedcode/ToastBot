#include "../Messaging/JsonProtocol.hpp"
#include "../Messaging/RestfulProtocol.hpp"
#include "Board.hpp"
#include "EspSpiffs.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "WebServerAdapter.hpp"

const int WebServerAdapter::MAX_WEBPAGES;

const long WebServerAdapter::RESPONSE_DELAY;

WebServerAdapter::WebServerAdapter(
   const String& id,
   const int& port) :
     Adapter(id, 0),
     port(port),
     server(0),
     responseTime(0)
{
   // Nothing to do here.
}

WebServerAdapter::WebServerAdapter(
   MessagePtr parameters) :
      Adapter(parameters),
      server(0),
      responseTime(0)
{
   port = parameters->getInt("port");
}

WebServerAdapter::~WebServerAdapter()
{
   // Nothing to do here.
}

void WebServerAdapter::setup()
{
   Adapter::setup();

   server = new EspWebServer(port);
   server->addHandler(this);
   server->begin();
}

void WebServerAdapter::loop()
{
   Adapter::loop();

   server->handleClient();

   // Process timeouts from RESTful requests.
   if (responseTime && (Board::getBoard()->systemTime() > responseTime))
   {
      handleNotFound(*server, pendingRequest.requestMethod, pendingRequest.requestUri);
      responseTime = 0;
   }
}

bool WebServerAdapter::sendRemoteMessage(
   MessagePtr message)
{
   static JsonProtocol jsonProtocol;

   server->send(200, "application/json", jsonProtocol.serialize(message));

   responseTime = 0;

   return (true);
}

MessagePtr WebServerAdapter::getRemoteMessage()
{
   // Nothing to do here.
   return (0);
}

bool WebServerAdapter::canHandle(
   HTTPMethod method,
   String uri)
{
   return (true);
}

bool WebServerAdapter::handle(
   EspWebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   bool success = true;

   Logger::logDebug(F("WebServerAdapter::handle: %s"), requestUri.c_str());

   success = servePage(server, requestMethod, requestUri);

   if (!success)
   {
      success = serveFile(server, requestMethod, requestUri);
   }

   if (!success)
   {
      success = serveRestfulRequest(server, requestMethod, requestUri);
   }

   if (!success)
   {
      handleNotFound(server, requestMethod, requestUri);
      success = true;
   }

   return (success);
}

void WebServerAdapter::handleNotFound(
   EspWebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   server.send(404, "text/plain", "File not found.");
}

void WebServerAdapter::addPage(
   Webpage* webpage)
{
   webpages.push_back(webpage);
}

Webpage* WebServerAdapter::getPage(
   const String& uri)
{
   Webpage* webpage = 0;

   for (List<Webpage*>::Iterator it = webpages.begin(); it != webpages.end(); it++)
   {
      Webpage* tempPage = (*it);
      if (tempPage->getUri() == uri)
      {
         webpage = tempPage;
         break;
      }
   }

   return (webpage);
}

void WebServerAdapter::removePage(
   const String& uri)
{
   Webpage* webpage = getPage(uri);

   if (webpage != 0)
   {
      webpages.remove(webpage);
   }
}

bool WebServerAdapter::servePage(
   EspWebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   bool success = false;

   static RestfulProtocol restfulProtocol;

   String content = "";

   Webpage* webpage = getPage(requestUri);

   if ((webpage != 0) && (webpage->canHandle(requestMethod, requestUri)))
   {
      Logger::logDebug(F("WebServerAdapter::servePage: %s"), requestUri.c_str());

      MessagePtr message = Messaging::newMessage();
      if (message && (restfulProtocol.parse(server, message)))
      {
         String contentFile = "";
         if (webpage->handle(requestMethod, requestUri, message, contentFile))
         {
            success = serveFile(server, requestMethod, contentFile);
         }
      }

      Messaging::freeMessage(message);
   }

   return (success);
}

bool WebServerAdapter::serveFile(
   EspWebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   bool success = false;

   // Determine the data type based on the file extension.
   String dataType = getDataType(requestUri);

   // Attempt to open the file from the SPIFFS file system.
   File file = SPIFFS.open(requestUri.c_str(), "r");

   if (file)
   {
      Logger::logDebug(F("WebServerAdapter::serveFile: %s"), requestUri.c_str());

      if (server.hasArg("download"))
      {
         dataType = "application/octet-stream";
      }

      // Send the file contents.
      if (server.streamFile(file, dataType) != file.size())
      {
         Logger::logWarning(F("WebServerAdapter::serveFile: Sent less data than expected."));
         success = true;
      }
      else
      {
         success = true;
      }

      file.close();
   }

   return (success);
}

bool WebServerAdapter::serveRestfulRequest(
   EspWebServer& server,
   HTTPMethod requestMethod,
   String requestUri)
{
   bool success = false;

   static RestfulProtocol restfulProtocol;

   MessagePtr message = Messaging::newMessage();
   if ((message) && (restfulProtocol.parse(server, message)))
   {
      Logger::logDebug(F("WebServerAdapter::serveRestfulRequest: %s"), requestUri.c_str());

      pendingRequest.requestMethod = requestMethod;
      pendingRequest.requestUri = requestUri;

      message->setSource(getId());
      Messaging::send(message);
      success = true;
   }

   // Allow some time for the component to respond.
   // Note: We'll send a 404 File Not Found message if not response has been returned.
   responseTime = Board::getBoard()->systemTime() + RESPONSE_DELAY;

   return (success);
}

String WebServerAdapter::getDataType(
   const String& path)
{
   String dataType = "text/plain";

   if ((path.endsWith(".htm")) ||
       (path.endsWith(".html")))
   {
      dataType = "text/html";
   }
   else if (path.endsWith(".css"))
   {
      dataType = "text/css";
   }
   else if (path.endsWith(".js"))
   {
      dataType = "application/javascript";
   }
   else if (path.endsWith(".png"))
   {
      dataType = "image/png";
   }
   else if (path.endsWith(".gif"))
   {
      dataType = "image/gif";
   }
   else if (path.endsWith(".jpg"))
   {
      dataType = "image/jpeg";
   }
   else if (path.endsWith(".ico"))
   {
      dataType = "image/x-icon";
   }
   else if (path.endsWith(".xml"))
   {
      dataType = "text/xml";
   }
   else if (path.endsWith(".pdf"))
   {
      dataType = "application/pdf";
   }
   else if (path.endsWith(".zip"))
   {
      dataType = "application/zip";
   }

   return (dataType);
}

