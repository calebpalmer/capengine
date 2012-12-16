#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

//@brief - Header file for EventDispatcher Class
//@author - Caleb Palmer

#include <vector>
#include <SDL/SDL.h>
#include "IEventSubscriber.h"

namespace CapEngine {
  const int mouseEvent =  0x01;
  const int keyboardEvent = 0x02;
  
  // Class to recieve game events and dispatch them to subscribed parties
  class EventDispatcher{
  private:
    typedef struct subscription_t {
      short subscriptionType;
      IEventSubscriber* subscriber;
    } subscription;

    std::vector<subscription>* subscribers;
    std::vector<SDL_Event*>* eventQueue;
    int queueDelayCount;

    EventDispatcher(const EventDispatcher&){}
    EventDispatcher& operator=(const EventDispatcher&){return *this;}

  public:
    EventDispatcher(int queueDelay = 0);
    ~EventDispatcher();

    void subscribe(IEventSubscriber* subscriber_in, int subscriptionMask);
    void enqueue(SDL_Event* event);
    void flushQueue();
    bool hasEvents();
    
  };

}
#endif
