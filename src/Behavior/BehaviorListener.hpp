#pragma once

class Behavior;

class BehaviorListener
{

public:

   virtual ~BehaviorListener() = 0;

   virtual void onStateChange(
      Behavior* behavior,
      const int& previousState,
      const int& newState) = 0;
};

inline BehaviorListener::~BehaviorListener() {}
