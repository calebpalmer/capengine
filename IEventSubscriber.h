#ifndef IEVENTSUBSCRIBER_H
#define IEVENTSUBSCRIBER_H

#include "Time.h"

//@brief - Interface for subscribing to EventManager
//@authoer - Caleb Palmer

class IEventSubscriber {
 public:
  virtual void receiveEvent(const SDL_Event* event, CapEngine::Time* time) = 0;
  virtual ~IEventSubscriber(){};
};

//IEventSubscriber::~IEventSubscriber(){};

#endif
