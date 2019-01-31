#include "Board.hpp"
#include "Button.hpp"
#include "CommonDefs.hpp"
#include "Logger.hpp"

static const bool BUTTON_DOWN = HIGH;
static const bool BUTTON_UP = LOW;

Button::Button(
   const String& id,
   const int& pin,
   const bool& inputPullup) :
      Sensor(id),
      pin(pin),
      inputPullup(inputPullup),
      longPressTime(0),
      buttonState(0),
      downTime(0)
{

}

Button::Button(
   MessagePtr message) :
      Sensor(message),
      buttonState(0),
      downTime(0)
{
   pin = message->isSet("pin") ? message->getInt("pin") : 0;
   inputPullup = message->isSet("inputPullup") ? message->getBool("inputPullup") : false;
   longPressTime = message->isSet("longPress") ? message->getInt("longPress") : 0;
}

Button::~Button()
{
   // Nothing to do here.
}

void Button::setLongPress(
   const int& longPressTime)
{
   this->longPressTime = longPressTime;
}

int Button::getLongPress() const
{
   return (longPressTime);
}

void Button::setup()
{
   Board* board = Board::getBoard();
   if (board)
   {
      if (inputPullup)
      {
         board->pinMode(pin, INPUT_PULLUP);
      }
      else
      {
         board->pinMode(pin, INPUT);
      }
   }
}

void Button::loop()
{
   Sensor::loop();

   bool wasDown = (buttonState == BUTTON_DOWN);
   bool isDown = (read() == BUTTON_DOWN);

   if (wasDown && !isDown)
   {
      onButtonUp();
   }
   else if (isDown)
   {
      Board* board = Board::getBoard();

      if (!wasDown && isDown)
      {
         onButtonDown();

         if (board)
         {
            downTime = board->systemTime();
         }
      }
      else if (board &&
               (longPressTime > 0) &&
               (downTime > 0) &&
               ((board->systemTime() - downTime) > longPressTime))
      {
         onButtonLongPress();
         downTime = 0;
      }
   }
}

void Button::handleMessage(
   MessagePtr message)
{
   if (message->getMessageId() == "longPress")
   {
      longPressTime = message->getInt("longPress");

      Logger::logDebug(F("Button::handleMessage: longPress(%d)"), longPressTime);
   }
   else if (message->getMessageId() == "poll")
   {
      int period = message->isSet("period") ? message->getInt("period") : 0;

      Logger::logDebug(F("Button::handleMessage: poll(%d)"), period);

      poll(period);
   }
   else
   {
      Component::handleMessage(message);
   }
}

int Button::read()
{
   Board* board = Board::getBoard();

   if (board)
   {
      buttonState = interpret(board->digitalRead(pin));
   }

   return (buttonState);
}

void Button::onPoll()
{
   Logger::logDebug("Button::onPoll");

   MessagePtr message = Messaging::newMessage();
   if (message)
   {
      message->setMessageId("buttonState");
      message->setTopic("sensor");
      message->setSource(getId());
      message->set("value", buttonState);
      Messaging::publish(message);
   }
}

void Button::onButtonDown()
{
   Logger::logDebug(F("Button::onButtonDown: %s"), getId().c_str());
   MessagePtr message = Messaging::newMessage();
   if (message)
   {
      message->setTopic("buttonDown");
      message->setSource(getId());
      Messaging::publish(message);
   }
}

void Button::onButtonUp()
{
   Logger::logDebug(F("Button::onButtonUp: %s"), getId().c_str());
   MessagePtr message = Messaging::newMessage();
   if (message)
   {
      message->setTopic("buttonUp");
      message->setSource(getId());
      Messaging::publish(message);
   }
}

void Button::onButtonLongPress()
{
   Logger::logDebug(F("Button::onButtonLongPress: %s"), getId().c_str());
   MessagePtr message = Messaging::newMessage();
   if (message)
   {
      message->setTopic("buttonLongPress");
      message->setSource(getId());
      Messaging::publish(message);
   }
}

bool Button::interpret(
   const int& reading) const
{
   // TODO:
   return (reading == LOW);
}

