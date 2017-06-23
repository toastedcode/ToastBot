// *****************************************************************************
// *****************************************************************************
// ToastBot.cpp
//
// Author: Jason Tost
// Date:   7.12.2016
//
// *****************************************************************************
// *****************************************************************************

#include "Arduino.h"
#include "FS.h"

#include "BasicMessage.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "SerialLogger.hpp"
#include "Timer.hpp"
#include "ToastBot.hpp"

const int ToastBot::MAX_COMPONENTS;

String ToastBot::id;

Properties ToastBot::properties;

bool ToastBot::initialized = false;

Set<Component*, ToastBot::MAX_COMPONENTS> ToastBot::components;

const String PROPERTIES_FILE = "/toastbot.properties";

const String ASCII_LOGO =
"............................................................\n"
"..............MMM...........................................\n"
"..............MMM......................MM$..................\n"
".MMMM.MMMMMM7.MMMMMMMM..MMMMMMM.. MM .MM~..MMMMMMM .MMMMMMM.\n"
"MM...MMM.. MM MMM ..MMM.MM...MMM...MMMM... MM ..MMM.MM  ....\n"
"MM...MM....MM.MMM...$MM.MM...,MM....MM~...:MMMMMMMM.IMMMMMM \n"
"MM...MMM.. MM.MMM...MMM.MM...MMM...MMMM... MM ...  .::...MMM\n"
"MM....MMMMMMM..MMMMMMM .MMMMMMM...MM. MM=..MMMMMMM. MMMMMMM.\n"
"............................................................\n";

bool ToastBot::add(
   Component* component,
   const bool& setDefaultHandler)
{
   bool isSuccess = false;

   if (components.add(component) == true)
   {
      MessageRouter::registerHandler(component, setDefaultHandler);

      if (initialized == true)
      {
         component->setup();
      }

      isSuccess = true;
   }

   return (isSuccess);
}

bool ToastBot::remove(
   Component* component)
{
   bool isSuccess = false;

   if (components.remove(component) == true)
   {
      MessageRouter::unregisterHandler(component);
      isSuccess = true;
   }

   return (isSuccess);
}

Component* ToastBot::get(
   const String& id)
{
   Component* component = 0;

   for (int i = 0; i < components.length(); i++)
   {
      if (components.item(i)->value->getId() == id)
      {
         component = components.item(i)->value;
         break;
      }
   }

   return (component);
}

const Properties& ToastBot::getProperties()
{
   return (properties);
}

void ToastBot::setup(
   Board* board)
{
#ifdef ARDUINO
   // Arduino library setup.
   Serial.begin(9600);
   SPIFFS.begin();
#endif

   // Logo.
   //Serial.printf("\n\n" + ASCII_LOGO + "\n");

   // Set the logger.
   Logger::setLogger(new SerialLogger());

   // Setup the board.
   // TODO: Read board from properties file and create dynamically.
   Board::setBoard(board);

   // Load properties.
   properties.load(PROPERTIES_FILE);
   Logger::logDebug("ToastBot::setup: Properties: \n%s", properties.toString().c_str());

   setId(properties.getString("deviceName"));

   //  Setup messaging.
   Messaging::setup<BasicMessage>(10);

   // Setup registered components.
   for (int i = 0; i < components.length(); i++)
   {
      components.item(i)->value->setup();
   }

   // Configure the connection manager.
   // TODO:
   /*
   Connection::setApConfig(properties.getString("ap.ssid"), properties.getString("ap.password"));
   Connection::setWifiConfig(properties.getString("wifi.ssid"), properties.getString("wifi.password"));
   Connection::setServerConfig(properties.getString("server.host"), properties.getString("server.clientId"), properties.getString("server.clientPassword"));
   Connection::setMode(fromString(properties.getString("mode")));
   */

   initialized = true;
}

void ToastBot::loop()
{
   Timer::loop();

   for (int i = 0; i < components.length(); i++)
   {
      components.item(i)->value->loop();
   }
}
