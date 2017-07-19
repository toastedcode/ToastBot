#include <FS.h>

#include "CommandProtocol.hpp"
#include "Board.hpp"
#include "Logger.hpp"
#include "Messaging.hpp"
#include "Program.hpp"

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
      Logger::logWarning("Program::load: Failed to load program file: %s", path.c_str());
   }
   else
   {
      Logger::logWarning("Program::load: Loading program file: %s", path.c_str());

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
   Logger::logDebug("Program::run: Running program %s", getId().c_str());

   instructionIndex = 0;
   running = true;
   this->loopProgram = loopProgram;
}

void Program::stop()
{
   Logger::logDebug("Program::stop: Stopping program %s", getId().c_str());

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
      if (message->getMessageId() == "wait")
      {
         waitTime = Board::getBoard()->systemTime() + (message->getInt("time") * 1000);
         success = true;
         message->setFree();
      }
      else
      {
         success = Messaging::send(message);
      }
   }
   else
   {
      Logger::logWarning("Failed to execute program [%s] instruction: %s", getId().c_str(), instruction.c_str());
   }

   return (success);
}

