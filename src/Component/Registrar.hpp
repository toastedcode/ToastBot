#include "Component.hpp"
#include "ComponentFactory.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class Registrar : public Component, TimerListener
{

public:

   Registrar(
      const String& id,
      const String& registryUrl,
      const int& refreshPeriod);

   Registrar(
      MessagePtr message);

   ~Registrar();

   void setup();

   void timeout(
      Timer* timer);

private:

   void pingRegistry();

   String adapterId;

   int refreshPeriod;

   Timer* timer;
};

REGISTER(Registrar, Registrar)
