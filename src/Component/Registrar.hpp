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

   virtual ~Registrar();

   void setup();

   void timeout(
      Timer* timer);

protected:

   String getAdapterId();

private:

   virtual void pingRegistry();

   String adapterId;

   int refreshPeriod;

   Timer* timer;
};

inline String Registrar::getAdapterId()
{
   return (adapterId);
}

REGISTER(Registrar, Registrar)
