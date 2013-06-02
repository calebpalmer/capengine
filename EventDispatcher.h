#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

//@brief - Header file for EventDispatcher Class
//@author - Caleb Palmer

#include <vector>
#include <memory>
#include <SDL/SDL.h>
#include "IEventSubscriber.h"

namespace CapEngine {
  const int mouseEvent =  0x01;
  const int keyboardEvent = 0x02;
  const int systemEvent = 0x04;
  
  // Class to recieve game events and dispatch them to subscribed parties
  class EventDispatcher{
  private:
    typedef struct subscription_t {
      short subscriptionType;
      IEventSubscriber* subscriber;
    } subscription;

    static std::unique_ptr<EventDispatcher> instance;
    std::vector<subscription>* subscribers;
    std::vector<SDL_Event*>* eventQueue;
    int queueDelayCount;

    EventDispatcher(int queueDelay = 0);
    EventDispatcher(const EventDispatcher&){}
    EventDispatcher& operator=(const EventDispatcher&){return *this;}
    SDL_Event* copyEvent(SDL_Event* event);

  public:
    ~EventDispatcher();
    static EventDispatcher& getInstance();
    void subscribe(IEventSubscriber* subscriber_in, int subscriptionMask);
    void enqueue(SDL_Event* event);
    void flushQueue();
    bool hasEvents();
    void getEvents();
    
  };

}
#endif
