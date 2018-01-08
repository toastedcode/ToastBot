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

Properties ToastBot::properties;

bool ToastBot::initialized = false;

Set<Component*> ToastBot::components;

const String PROPERTIES_FILE = "/robox.properties";
const String DEFAULT_PROPERTIES_FILE = "/default.properties";

const char PROGMEM ASCII_LOGO[] =
"............................................................\n"
"..............MMM...........................................\n"
"..............MMM......................MM$..................\n"
".MMMM.MMMMMM7.MMMMMMMM..MMMMMMM.. MM .MM~..MMMMMMM .MMMMMMM.\n"
"MM...MMM.. MM MMM ..MMM.MM...MMM...MMMM... MM ..MMM.MM  ....\n"
"MM...MM....MM.MMM...$MM.MM...,MM....MM~...:MMMMMMMM.IMMMMMM \n"
"MM...MMM.. MM.MMM...MMM.MM...MMM...MMMM... MM ...  .::...MMM\n"
"MM....MMMMMMM..MMMMMMM .MMMMMMM...MM. MM=..MMMMMMM. MMMMMMM.\n"
"............................................................\n";

// The number of simultaneous messages supported by the Messaging framework.
const int MESSAGE_POOL_SIZE = 10;

// The number of simultaneous timers supported by the Timer framework.
const int TIMER_POOL_SIZE = 10;

// The GPIO pin used to control the status LED.
const int STATUS_LED_PIN = 16;

// *****************************************************************************
//                               Public
// *****************************************************************************

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
   bool isSuccess = true;

   components.insert(component);
   if (true)  // TODO: Change insert() to return an iterator and a bool
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

   components.erase(component);
   if (true)  // TODO: Change erase() to return the number of elements erased
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

   for (Set<Component*>::Iterator it = components.begin(); it != components.end(); it++)
   {
      if ((*it)->getId() == id)
      {
         component = (*it);
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

   ESP.wdtDisable();  // To prevent WDT reset
#endif

   // Logo.
   Serial.print("\n\n");
   Serial.print(FPSTR(ASCII_LOGO));
   Serial.print("\n");

   // Set the logger.
   Logger::setLogger(new SerialLogger());

   // Create the timer pool.
   Timer::setup(TIMER_POOL_SIZE);

   // Load properties.
   properties.load(PROPERTIES_FILE);
   Logger::logDebug(F("ToastBot::setup: Properties:"));
   properties.log();

   // Set log levels.
   if (properties.isSet("logLevel"))
   {
      Logger::setLogLevel(fromString(properties.getString("logLevel")));
   }

   // Setup the board.
   // TODO: Read board from properties file and create dynamically.
   Board::setBoard(board);

   //  Setup messaging.
   Messaging::setup<BasicMessage>(MESSAGE_POOL_SIZE);

   // Creating basic messaging adapters.
   Protocol* protocol = new JsonProtocol();
   addComponent(new SerialAdapter("serial", protocol));
   if (properties.isSet("discoverPort"))
   {
      addComponent(new UdpAdapter("discover", protocol, properties.getInt("discoverPort")));
   }
   if (properties.isSet("controlPort"))
   {
      addComponent(new TcpServerAdapter("control", protocol, properties.getInt("controlPort")));
   }
   if (properties.isSet("debugPort"))
   {
      addComponent(new TcpServerAdapter("debug", protocol, properties.getInt("debugPort")));
   }
   if (properties.isSet("server.host") && properties.isSet("server.port"))
   {
      addComponent(new MqttClientAdapter("online", protocol));
   }

   // Factory reset button.
   // TODO: Flash button conflicts with motor1 pin.
   Button* flashButton = new FactoryResetButton("flashButton", 0);
   //flashButton->setLongPress(5000);
   addComponent(flashButton);

   // Status LED.
   addComponent(new Led("statusLed", STATUS_LED_PIN));

   //
   // Create components found in properties.
   //

   Message* message = MessageFactory::newMessage();

   Set<String> propertyKeys;
   properties.getKeys("component", propertyKeys);

   for (Set<String>::Iterator it = propertyKeys.begin(); it != propertyKeys.end(); it++)
   {
      String componentDescription = ToastBot::getProperties().getString(*it);

      if (protocol->parse(componentDescription, message))
      {
        Logger::logDebug(F("ToastBot::setup: Creating %s component [%s]"),
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
   for (Set<Component*>::Iterator it = components.begin(); it != components.end(); it++)
   {
      (*it)->setup();
   }

   // Configure the connection manager.
   configureConnections();

   // Log free memory.
   Logger::logDebug(F("ToastBot::setup: Free memory = %u bytes"), board->getFreeHeap());

   initialized = true;

#ifdef ARDUINO
   ESP.wdtEnable(1000);
#endif
}

void ToastBot::loop()
{
   Timer::loop();

   for (Set<Component*>::Iterator it = components.begin(); it != components.end(); it++)
   {
      (*it)->loop();
   }
}

void ToastBot::factoryReset()
{
   Logger::logDebug(F("ToastBot::factoryReset: Resetting device back to factory default."));

   Properties defaultProperties;
   defaultProperties.load(DEFAULT_PROPERTIES_FILE);

   defaultProperties.saveAs(PROPERTIES_FILE);

   Board* board = Board::getBoard();
   if (board)
   {
      board->reset();
   }
}

// *****************************************************************************
//                               Private
// *****************************************************************************

void ToastBot::configureConnections()
{
   // Configure the AP.
   if (properties.isSet("ap.ssid"))
   {
      Connection::setApConfig(properties.getString("ap.ssid"), properties.getString("ap.password"));
   }
   else
   {
      Connection::setApConfig(getId(), "");
   }

   // Configure the Wifi connection.
   Connection::setWifiConfig(properties.getString("wifi.ssid"), properties.getString("wifi.password"));

   // Configure the server connection.
   Connection::setServerConfig(
      properties.getString("server.host"),
      properties.getInt("server.port"),
      properties.getString("server.userId"),
      properties.getString("server.password"),
      getUniqueId(),  // clientId
      properties.getString("server.topic"));

   MqttClientAdapter* onlineAdapter = (MqttClientAdapter*)getComponent("online");
   if (onlineAdapter)
   {
      ServerConfig serverConfig = Connection::getServerConfig();

      // MQTT topic format:
      // Sending:     /robox/user-supplied-topic/robox-id/from
      // Subscribing: /robox/user-supplied-topic/robox-id/to
      String topic = "robox/" + serverConfig.topic + "/" + getId();

      onlineAdapter->setServer(serverConfig.host, serverConfig.port);
      onlineAdapter->setClientId(serverConfig.clientId);
      onlineAdapter->setUser(serverConfig.userId, serverConfig.password);
      onlineAdapter->setTopic(topic);

      Connection::setOnlineAdapter(onlineAdapter);
   }
   else
   {
      Logger::logWarning(F("ToastBot::configureConnections: No online adapter available."));
   }

   // Configure the status LED.
   Led* statusLed = (Led*)getComponent("statusLed");
   if (statusLed)
   {
      Connection::setStatusLed(statusLed);
   }

   // Set the connection mode and attempt to make connections.
   Connection::setMode(parseConnectionMode(properties.getString("mode")));
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
