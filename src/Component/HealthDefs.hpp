#pragma once

enum Health
{
   HEALTH_FIRST,
   HEALTH_UNKNOWN = HEALTH_FIRST,
   RECOVERING,
   HEALTHY,
   FAILING,
   FAILED,
   HEALTH_LAST
};

inline String toString(
   const Health& mode)
{
   static const String enumToString[]
   {
      "UNKNOWN",
      "RECOVERING",
      "HEALTHY",
      "FAILING",
      "FAILED",
   };

   return (enumToString[mode]);
}

inline Health parseHealth(
   const String& healthString)
{
   Health health = HEALTH_UNKNOWN;

   for (int i = HEALTH_FIRST; i < HEALTH_LAST; i++)
   {
      if (healthString == toString(static_cast<Health>(i)))
      {
         health = static_cast<Health>(i);
         break;
      }
   }

   return (health);
}
