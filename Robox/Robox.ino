#include <Board.h>
#include <ToastBot.h>

#include "ConfigPage.hpp"
#include "Robox.hpp"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

void setup()
{
   ToastBot::setup(new Esp8266Board());

   ToastBot::addComponent(new Robox(), true);  // <-- default handler

   WebServerAdapter* webServerAdapter = new WebServerAdapter("web", 80);
   ToastBot::addComponent(webServerAdapter);
   webServerAdapter->addPage(new ConfigPage());
}

void loop()
{
   ToastBot::loop();
}
