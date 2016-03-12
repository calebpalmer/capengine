#ifndef IEVENTSUBSCRIBER_H
#define IEVENTSUBSCRIBER_H

#include <SDL2/SDL.h>

#include "Time.h"

//@brief - Interface for subscribing to EventManager
//@authoer - Caleb Palmer
namespace CapEngine {
  class IEventSubscriber {
  public:
    virtual void receiveEvent(SDL_Event event, CapEngine::Time* time) = 0;
    virtual ~IEventSubscriber(){};
  };
}

//IEventSubscriber::~IEventSubscriber(){};

#endif
