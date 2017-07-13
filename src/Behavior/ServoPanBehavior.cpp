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
   // ocillate
   else if (message->getMessageId() == "ocillate")
   {
      int angle = message->getInt("angle");
      int seconds = message->getInt("seconds");

      ocillate(angle, seconds);

      message->setFree();
   }
   // stop
   else if (message->getMessageId() == "stop")
   {
      stop();

      message->setFree();
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
         switch (getState())
         {
            case PAN:
            {
               setState(DONE);
               break;
            }

            case OCILLATE:
            {
               // Swap start/end pwm.
               int tempPwm = startPwm;
               startPwm = endPwm;
               endPwm = tempPwm;

               // Set new start/end times.
               unsigned long duration = (endTime = startTime);
               startTime = Board::getBoard()->systemTime();
               endTime = (startTime + duration);
               break;
            }

            default:
            {
               break;
            }
         }
      }
   }
}

void ServoPanBehavior::panTo(
   const int& angle,
   const int& seconds,
   const bool& ocillate)
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

void ServoPanBehavior::ocillate(
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

      setState(OCILLATE);
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
