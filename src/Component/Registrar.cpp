#include "Esp8266Board.hpp"
#include "Registrar.hpp"
#include "ToastBot.hpp"
#include "WifiBoard.hpp"

Registrar::Registrar(
   const String& id,
   const String& adapterId,
   const int& refreshPeriod) :
      Component(id),
      adapterId(adapterId),
      refreshPeriod(refreshPeriod),
      timer(0)
{
}

Registrar::Registrar(
   MessagePtr message) :
      Component(message),
      timer(0)
{
   adapterId = message->getString("adapterId");
   refreshPeriod = message->getInt("refreshPeriod");
}

Registrar::~Registrar()
{
   Timer::freeTimer(timer);
}

void Registrar::setup()
{
   timer = Timer::newTimer((getId() + ".timer"), refreshPeriod, Timer::PERIODIC, this);
   timer->start();
}

void Registrar::timeout(
   Timer* timer)
{
   pingRegistry();
}

void Registrar::pingRegistry()
{
   if (WifiBoard::getBoard())
   {
      MessagePtr message = Messaging::newMessage();

      if (message)
      {
         message->setMessageId("register");
         message->setDestination(adapterId);

         // Get the MAC address.
         unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
         WifiBoard::getBoard()->getMacAddress(mac);
         char macAddress[18];
         sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

         // Determine the chip id.
         int chipId = 0;
         if (Board::getBoard()->getBoardName() == "ESP8266")
         {
            chipId = static_cast<Esp8266Board*>(Board::getBoard())->getChipId();
         }
         else
         {
           // TODO: Derive chip id from MAC address.
         }

         message->set("chipId", chipId);
         message->set("roboxName", ToastBot::getId());
         message->set("userId", ToastBot::getProperties().getString("userId"));
         message->set("ipAddress", WifiBoard::getBoard()->getIpAddress());
         message->set("macAddress", macAddress);

         Messaging::send(message);
      }
   }
}
