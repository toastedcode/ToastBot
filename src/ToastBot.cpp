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
#include "Connection/Connection.hpp"
#include "Component/FactoryResetButton.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "SerialLogger.hpp"
#include "Timer.hpp"
#include "ToastBot.hpp"
#include "WifiBoard.hpp"

const int ToastBot::MAX_COMPONENTS;

Properties ToastBot::properties;

bool ToastBot::initialized = false;

Set<Component*, ToastBot::MAX_COMPONENTS> ToastBot::components;

const String PROPERTIES_FILE = "/robox.properties";
const String DEFAULT_PROPERTIES_FILE = "/default.properties";

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

String ToastBot::getId()
{
   String id = "";

   if (properties.isSet("deviceName"))
   {
      id = properties.getString("deviceName");
   }
   else
   {
      id = getUniqueId();
   }

   return (id);
}

bool ToastBot::addComponent(
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

bool ToastBot::removeComponent(
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

Component* ToastBot::getComponent(
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

Properties& ToastBot::getProperties()
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
   Serial.print("\n\n" + ASCII_LOGO + "\n");

   // Set the logger.
   Logger::setLogger(new SerialLogger());

   // Load properties.
   properties.load(PROPERTIES_FILE);
   Logger::logDebug("ToastBot::setup: Properties: \n%s", properties.toString().c_str());

   // Setup the board.
   // TODO: Read board from properties file and create dynamically.
   Board::setBoard(board);

   //  Setup messaging.
   Messaging::setup<BasicMessage>(10);

   // Creating basic messaging adapters.
   Protocol* protocol = new JsonProtocol();
   addComponent(new SerialAdapter("serial", protocol));
   addComponent(new UdpAdapter("discover", protocol, properties.getInt("discoverPort")));
   addComponent(new TcpServerAdapter("control", protocol, properties.getInt("controlPort")));
   addComponent(new TcpServerAdapter("debug", protocol, properties.getInt("debugPort")));

   // Factory reset button.
   // TODO: Flash button conflicts with motor1 pin.
   Button* flashButton = new FactoryResetButton("flashButton", 0);
   //flashButton->setLongPress(5000);
   addComponent(flashButton);

   // Status LED.
   Led* statusLed = new Led("statusled", 16);
   addComponent(statusLed);

   // Create components found in properties.
   Message* message = MessageFactory::newMessage();
   String componentIds[50];
   int count = 0;
   ToastBot::getProperties().getKeys("component", componentIds, count);
   for (int i = 0; i < count; i++)
   {
      String componentDescription = ToastBot::getProperties().getString(componentIds[i]);

      if (protocol->parse(componentDescription, message))
      {
        Logger::logDebug("ToastBot::setup: Creating %s component [%s]",
                         message->getString("class").c_str(),
                         message->getString("id").c_str());

        Component* component = ComponentFactory::create(message->getString("class"), message);
        if (component)
        {
          ToastBot::addComponent(component);
        }
      }
   }
   message->setFree();

   // Setup registered components.
   for (int i = 0; i < components.length(); i++)
   {
      components.item(i)->value->setup();
   }

   // Configure the connection manager.
   if (properties.isSet("ap.ssid"))
   {
      Connection::setApConfig(properties.getString("ap.ssid"), properties.getString("ap.password"));
   }
   else
   {
      Connection::setApConfig(getId(), "");
   }
   Connection::setWifiConfig(properties.getString("wifi.ssid"), properties.getString("wifi.password"));
   Connection::setServerConfig(properties.getString("server.host"), properties.getString("server.clientId"), properties.getString("server.clientPassword"));
   Connection::setStatusLed(statusLed);
   Connection::setMode(parseConnectionMode(properties.getString("mode")));

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

void ToastBot::factoryReset()
{
   Logger::logDebug("ToastBot::factoryReset: Resetting device back to factory default.");

   Properties defaultProperties;
   defaultProperties.load(DEFAULT_PROPERTIES_FILE);

   defaultProperties.saveAs(PROPERTIES_FILE);

   Board* board = Board::getBoard();
   if (board)
   {
      board->reset();
   }
}

String ToastBot::getUniqueId()
{
   static const String PREFIX = "ROBOX";

   String uniqueId = "";
   char buffer[32];

   WifiBoard* board = WifiBoard::getBoard();

   if (board)
   {
      // Retrieve the MAC address of the board.
      unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
      board->getMacAddress(mac);

      // Make an id out of the MAC address.
      sprintf(buffer,
              "%s_%02X%02X%02X%02X%02X%02X",
              PREFIX.c_str(),
              mac[0],
              mac[1],
              mac[2],
              mac[3],
              mac[4],
              mac[5]);
   }
   else
   {
#ifdef ARDUINO
      // Seed the random number generator.
      randomSeed(analogRead(0));

      // Make an id out of some random numbers.
      sprintf(buffer,
              "%s_%02X%02X%02X%02X%02X%02X",
              PREFIX.c_str(),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255),
              random(255));
#endif
   }

   return (String(buffer));
}
