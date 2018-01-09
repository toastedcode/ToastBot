#include "../../archive/Program/Program.hpp"

#include <FS.h>

#include "../Messaging/CommandProtocol.hpp"
#include "Board.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"

// *****************************************************************************
//                                Public
// *****************************************************************************

Program::Program(
   const String& id) :
   Component(id),
   instructionIndex(0),
   instructionCount(0),
   running(false),
   waitTime(0),
   loopProgram(false)
{

}

Program::~Program()
{

}

void Program::setup()
{
   Component::setup();
}

void Program::loop()
{
   Component::loop();

   if (running)
   {
      if (waitTime != 0)
      {
         if (Board::getBoard()->systemTime() > waitTime)
         {
            waitTime = 0;
         }
      }

      if (!waitTime)
      {
         execute(instructions[instructionIndex]);
         instructionIndex++;

         if (instructionIndex >= instructionCount)
         {
            instructionIndex = 0;
            running = loopProgram;
         }
      }
   }
}

bool Program::load(
   const String& path)
{
   bool success = false;

   File file = SPIFFS.open(path, "r");
   if (!file)
   {
      Logger::logWarning(F("Program::load: Failed to load program file: %s"), path.c_str());
   }
   else
   {
      Logger::logWarning(F("Program::load: Loading program file: %s"), path.c_str());

      int index = 0;
      String line = file.readStringUntil('\n');

      while ((line.length() > 0) &&
             (index < MAX_INSTRUCTIONS))
      {
         instructions[index] = line;
         index++;

         line = file.readStringUntil('\n');
      }

      instructionCount = index;
      instructionIndex = 0;

      success = true;
   }

   return (success);

}

void Program::run(
   const bool& loopProgram)
{
   Logger::logDebug(F("Program::run: Running program %s"), getId().c_str());

   instructionIndex = 0;
   running = true;
   this->loopProgram = loopProgram;
}

void Program::stop()
{
   Logger::logDebug(F("Program::stop: Stopping program %s"), getId().c_str());

   instructionIndex = 0;
   running = false;
}

bool Program::execute(
   const String& instruction)
{
   static CommandProtocol protocol;

   bool success = false;

   MessagePtr message = Messaging::newMessage();

   if (message && protocol.parse(instruction, message))
   {
      if (message->getString("action") == "wait")
      {
         int seconds = message->getInt("param_0");

         Logger::logDebug(F("Program::execute: wait(%d)"), seconds);

         waitTime = Board::getBoard()->systemTime() + (seconds * 1000);
         success = true;
         Messaging::freeMessage(message);
      }
      else
      {
         success = Messaging::send(message);
      }
   }
   else
   {
      Logger::logWarning(F("Failed to execute program [%s] instruction: %s"), getId().c_str(), instruction.c_str());
   }

   return (success);
}

