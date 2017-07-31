#include "Board.hpp"
#include "CommonDefs.hpp"
#include "ComponentFactory.hpp"
#include "Led.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"

static const bool CONTINUE_PATTERN = false;

const int Led::MIN_BRIGHTNESS;

const int Led::MAX_BRIGHTNESS;

const int Led::MIN_PWM;

const int Led::MAX_PWM;

// *****************************************************************************

const int Led::LedPattern::PATTERN_UPDATE_PERIOD = 250;  // milliseconds

const int Led::LedPattern::MAX_PATTERN_LENGTH;

Led::LedPattern::LedPattern(
   Led* led,
   String patternString) :
      led(led),
      patternIndex(0),
      updateTime(0)

{
   setPattern(patternString);
}

Led::LedPattern::~LedPattern()
{
   // Nothing to do here.
}

void Led::LedPattern::setPattern(
   const String& patternString)
{
   memset(patternArray, 0, sizeof(patternArray));
   patternArraySize = patternString.length();

   if (patternArraySize > 0)
   {
      parse(patternString);
      updateTime = 0;
   }
}

void Led::LedPattern::update()
{
   if (patternArraySize > 0)
   {
      long systemTime = Board::getBoard()->systemTime();

      if (systemTime >= updateTime)
      {
         led->setBrightness(getNext(), CONTINUE_PATTERN);
         updateTime = systemTime + PATTERN_UPDATE_PERIOD;
      }
   }
}

int Led::LedPattern::getNext()
{
   patternIndex++;
   if (patternIndex >= patternArraySize)
   {
      patternIndex = 0;
   }

   return (patternArray[patternIndex]);
}

void Led::LedPattern::parse(String patternString)
{
   for (int i = 0; i < patternArraySize; i++)
   {
      char token = patternString.charAt(i);

      switch (token)
      {
         case '-':
         {
            patternArray[i] = MAX_BRIGHTNESS;
            break;
         }

         case '/':
         {
            int count = 0;
            int j = i;
            do
            {
               j++;
               count++;
            } while ((j < patternString.length()) &&
                     (patternString.charAt(j) == token));

            int delta = (MAX_BRIGHTNESS / (count + 1));

            for (int j = 0; j < count; j++)
            {
               patternArray[i + j] = MIN_BRIGHTNESS + (delta * (j + 1));
            }
            break;
         }

         case '\\':
         {
            int count = 0;
            int j = i;
            do
            {
               j++;
               count++;
            } while ((j < patternString.length()) &&
                     (patternString.charAt(j) == token));

            int delta = (MAX_BRIGHTNESS / (count + 1));

            for (int j = 0; j < count; j++)
            {
               patternArray[i + j] = MAX_BRIGHTNESS - (delta * (j + 1));
            }
            break;
         }

         case '_':
         default:
         {
            patternArray[i] = MIN_BRIGHTNESS;
            break;
         }
      }
   }
}

// *****************************************************************************

Led::LedPulse::LedPulse(
   Led* led,
   const int& period) :
      led(led),
      period(period)
{
}

Led::LedPulse::~LedPulse()
{

}

void Led::LedPulse::setPeriod(
   const int& period)
{
   this->period = period;
}

void Led::LedPulse::update()
{
   if (period > 0)
   {
      long halfPeriod = (period / 2);

      long systemTime = Board::getBoard()->systemTime();

      long periodTime = (systemTime % period);

      int brightness = 0;

      if (periodTime < halfPeriod)
      {
         // Rising
         brightness = map(periodTime, 0, halfPeriod, Led::MIN_BRIGHTNESS, Led::MAX_BRIGHTNESS);
      }
      else
      {
         // Falling
         brightness = map(periodTime, halfPeriod, period, Led::MAX_BRIGHTNESS, Led::MIN_BRIGHTNESS);
      }

      led->setBrightness(brightness, CONTINUE_PATTERN);
   }
}

// *****************************************************************************

Led::Led(
   const String& id,
   const int& pin) :
      Component(id),
      pin(pin),
      brightness(0),
      ledPattern(0),
      ledPulse(0)
{
   ledPattern = new LedPattern(this, "");
   ledPulse = new LedPulse(this, 0);
}

Led::Led(
   MessagePtr message) :
      Component(message),
      brightness(0)
{
   id = message->isSet("id") ? message->getString("id") : "led";
   pin = message->isSet("pin") ? message->getInt("pin") : 0;

   ledPattern = new LedPattern(this, "");
   ledPulse = new LedPulse(this, 0);
}

Led::~Led()
{
   // Nothing to do here.
}

void Led::setBrightness(
   const int& newBrightness,
   const bool& stopPattern)
{
   // Stop any ongoing pattern/pulse.
   if (stopPattern)
   {
      ledPattern->setPattern("");
      ledPulse->setPeriod(0);
   }

   brightness = constrain(newBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);

   int pwm = map(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS, MIN_PWM, MAX_PWM);

   Board::getBoard()->analogWrite(pin, pwm);
}

void Led::blink(
   const String& patternString)
{
   int foundPos = StringUtils::findFirstNotOf(patternString, "_-/\\", 0);

   if (foundPos != -1)
   {
      Logger::logWarning(F("Led::blink: Invalid pattern specified [\"%s\"]"), patternString.c_str());
   }
   else
   {
      ledPulse->setPeriod(0);

      ledPattern->setPattern(patternString);
   }
}

void Led::pulse(
   const int& period)
{
   ledPattern->setPattern("");

   ledPulse->setPeriod(period);
}

void Led::setup()
{
   Board::getBoard()->pinMode(pin, OUTPUT);

   Messaging::subscribe(this, "killSwitch");
}

void Led::loop()
{
   Component::loop();

   if (ledPattern)
   {
      ledPattern->update();
   }

   if (ledPulse)
   {
      ledPulse->update();
   }
}

void Led::handleMessage(
   MessagePtr message)
{
   // setBrightness
   if (message->getMessageId() == "setBrightness")
   {
      brightness = message->getInt("brightness");

      Logger::logDebug(F("Led::handleMessage: setBrightness(%d)"), brightness);

      setBrightness(brightness);

      message->setFree();
   }
   // blink
   else if (message->getMessageId() == "blink")
   {
      String patternString = message->getString("pattern");

      Logger::logDebug(F("Led::handleMessage: blink(\"%s\")"), patternString.c_str());

      blink(patternString);

      message->setFree();
   }
   // pulse
   else if (message->getMessageId() == "pulse")
   {
      int period = message->getInt("period");

      Logger::logDebug(F("Led::handleMessage: pulse(\"%d\")"), period);

      pulse(period);

      message->setFree();
   }
   // killSwitch
   else if (message->getMessageId() == "killSwitch")
   {
      Logger::logDebug(F("Led::handleMessage: killSwitch"));

      setBrightness(MIN_BRIGHTNESS);

      message->setFree();
   }
   // instruction
   else if (message->getMessageId() == "instruction")
   {
      String action = message->getString("action");

      if (action == "on")
      {
         int brightness = message->isSet("param_0") ? message->getInt("param_0") : 100;

         Logger::logDebug(F("Led::handleMessage: instruction:on(%d)"), brightness);

         setBrightness(brightness);
      }
      else if (action == "off")
      {
         Logger::logDebug(F("Led::handleMessage: instruction:off()"));

         setBrightness(0);
      }
      else if (action == "blink")
      {
         String patternString = message->getString("param_0");

         Logger::logDebug(F("Led::handleMessage: instruction:blink(%s)"), patternString.c_str());

         blink(patternString);
      }
      else if (action == "pulse")
      {
         int period = message->getInt("param_0");

         Logger::logDebug(F("Led::handleMessage: instruction:pulse(%d)"), period);

         pulse(period);
      }
      else
      {
         Logger::logWarning(F("Led::handleMessage: Illegal instruction [%s] for %s."),
                            message->getString("action").c_str(),
                            getId().c_str());
      }

      message->setFree();
   }
   else
   {
      Component::handleMessage(message);
   }
}
