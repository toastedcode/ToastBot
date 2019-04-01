#include <ToastBot.h>

#include "PropertiesPage.hpp"
#include "WebServer.hpp"

WebServer webServer(80);

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup();

   webServer.setup();
   webServer.addPage(new PropertiesPage());
}

void loop()
{
   ToastBot::loop();

   webServer.loop();
}
