#include <ToastBot.h>

#include "PropertiesPage.hpp"
#include "Robox.hpp"
#include "WebServer.hpp"

WebServer webServer(80);

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup();

   ToastBot::addComponent(new Robox(), true);  // <-- default handler

   webServer.setup();
   webServer.addPage(new PropertiesPage());
}

void loop()
{
   ToastBot::loop();

   webServer.loop();
}
