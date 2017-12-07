#include "Component.hpp"

class Program : public Component
{

public:

   Program(
      const String& id);

   virtual ~Program();

   virtual void setup();

   virtual void loop();

   bool load(
      const String& path);

   void run(
      const bool& loop);

   void stop();

private:

   bool execute(
      const String& instruction);

   static const int MAX_INSTRUCTIONS = 20;

   String instructions[MAX_INSTRUCTIONS];

   int instructionIndex;

   int instructionCount;

   unsigned long waitTime;

   bool running;

   bool loopProgram;
};
