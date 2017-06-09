#include "Board.hpp"
#include "ServoPanBehavior.hpp"

ServoPanBehavior::ServoPanBehavior(
   const String& id,
   ServoComponent* servo) :
      Behavior(id),
      servo(servo),
      startPwm(0),
      endPwm(0),
      startTime(0),
      endTime(0)
{
   enable();
}

ServoPanBehavior::~ServoPanBehavior()
{
}

void ServoPanBehavior::handleMessage(
   MessagePtr message)
{
   // panTo
   if (message->getMessageId() == "panTo")
   {
      int angle = message->getInt("angle");
      int seconds = message->getInt("seconds");

      panTo(angle, seconds);

      message->setFree();
   }
   // stop
   else if (message->getMessageId() == "stop")
   {
      stop();
   }
   else
   {
      Behavior::handleMessage(message);
   }
}

void ServoPanBehavior::loop()
{
   if (isEnabled() && (startTime != 0) && (getState() == PAN))
   {
      unsigned long currentTime = Board::getBoard()->systemTime();

      int deltaTime = (currentTime - startTime);

      int currentPwm = map(deltaTime, startTime, endTime, startPwm, endPwm);

      servo->setPwm(currentPwm);

      if (currentTime >= endTime)
      {
         setState(DONE);
      }
   }
}

void ServoPanBehavior::panTo(
   const int& angle,
   const int& seconds)
{
   static const int MILLIS_PER_SECOND = 1000;

   if (isEnabled())
   {
      startPwm = ServoComponent::angleToPwm(servo->getAngle());
      endPwm = ServoComponent::angleToPwm(angle);
      startTime = Board::getBoard()->systemTime();
      endTime = (startTime + (seconds * MILLIS_PER_SECOND));

      setState(PAN);
   }
}

void ServoPanBehavior::stop()
{
   if (isEnabled())
   {
      startPwm = 0;
      endPwm = 0;
      startTime = 0;
      endTime = 0;
   }
}
