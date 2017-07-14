#include "Board.hpp"
#include "Logger.hpp"
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

      Logger::logDebug("ServoPanBehavior::handleMessage: panTo(%d, %d)", angle, seconds);

      panTo(angle, seconds);

      message->setFree();
   }
   // ocillate
   else if (message->getMessageId() == "oscillate")
   {
      int startAngle = message->getInt("startAngle");
      int endAngle = message->getInt("endAngle");
      int seconds = message->getInt("seconds");

      Logger::logDebug("ServoPanBehavior::handleMessage: oscillate(%d, %d, %d)", startAngle, endAngle, seconds);

      oscillate(startAngle, endAngle, seconds);

      message->setFree();
   }
   // stop
   else if (message->getMessageId() == "stop")
   {
	  Logger::logDebug("ServoPanBehavior::handleMessage: stop()");

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
   if (isEnabled() &&
	   (startTime != 0) &&
	   ((getState() == PAN) || (getState() == OSCILLATE)))
   {
      unsigned long currentTime = Board::getBoard()->systemTime();

      int currentPwm = map(currentTime, startTime, endTime, startPwm, endPwm);

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

            case OSCILLATE:
            {
               // Swap start/end pwm.
               int tempPwm = startPwm;
               startPwm = endPwm;
               endPwm = tempPwm;

               // Set new start/end times.
               unsigned long duration = (endTime - startTime);
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

void ServoPanBehavior::oscillate(
   const int& startAngle,
   const int& endAngle,
   const int& seconds)
{
   static const int MILLIS_PER_SECOND = 1000;

   if (isEnabled())
   {
      startPwm = ServoComponent::angleToPwm(startAngle);
      endPwm = ServoComponent::angleToPwm(endAngle);
      startTime = Board::getBoard()->systemTime();
      endTime = (startTime + (seconds * MILLIS_PER_SECOND));

      setState(OSCILLATE);
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
