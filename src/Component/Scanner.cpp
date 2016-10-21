#include "Logger.hpp"
#include "Scanner.hpp"

const int Scanner::SERVO_POSITIONS;

const int Scanner::MIN_SERVO_POSITION;

const int Scanner::MAX_SERVO_POSITION;

const int Scanner::SERVO_ROTATE_PERIOD;

Scanner::Scanner(
   const String& id,
   ServoComponent* servo,
   DistanceSensor* sensor) : Component(id),
                             servo(servo),
                             sensor(sensor),
                             servoPosition(0),
                             servoDirection(RIGHT),
                             servoTimer(0)
{
   memset(reading, 0, sizeof(ScannerReading));
}

Scanner::~Scanner()
{
   Timer::freeTimer(servoTimer);
}

void Scanner::setup()
{
   servo->rotate(getServoAngle(servoPosition));

   servoTimer = Timer::newTimer("servoTimer", SERVO_ROTATE_PERIOD, Timer::PERIODIC, this);
   servoTimer->start();

   Component::setup();
}

void Scanner::handleMessage(
   MessagePtr message)
{
   Component::handleMessage(message);
}

void Scanner::timeout(
   Timer* timer)
{
   // Take a reading (in centimeters) at the current position.
   reading[servoPosition] = DistanceSensor::toCentimeters(sensor->read());

   Logger::logDebug("%d, %d, %d, %d, %d", reading[0], reading[1], reading[2], reading[3], reading[4]);

   // Rotate to the new position.
   if (servoDirection == RIGHT)
   {
      servoPosition++;
      if (servoPosition == MAX_SERVO_POSITION)
      {
         servoDirection = LEFT;
      }
   }
   else
   {
      servoPosition--;
      if (servoPosition == MIN_SERVO_POSITION)
      {
         servoDirection = RIGHT;
      }
   }

   servo->rotate(getServoAngle(servoPosition));
}

inline void Scanner::read(
   ScannerReading& reading)
{
   memcpy(reading, this->reading, sizeof(ScannerReading));
}

int Scanner::getServoAngle(
   const int& position)
{
   static const int MIN_ANGLE = 0;
   static const int MAX_ANGLE = 180;

   static ScannerReading servoAngles;
   static bool initialized = false;

   // Initialize an array mapping servo position to servo angle.
   if (!initialized)
   {
      for (int i = 0; i < SERVO_POSITIONS; i++)
      {
         servoAngles[i] = (MIN_ANGLE + (i * ((MAX_ANGLE - MIN_ANGLE) / (SERVO_POSITIONS - 1))));
      }

      initialized = true;
   }

   return (servoAngles[position]);
}
