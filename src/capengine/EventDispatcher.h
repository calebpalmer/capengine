#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "IEventSubscriber.h"
#include "VideoManager.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

namespace CapEngine
{
const int mouseEvent = 0x01;
const int keyboardEvent = 0x02;
const int systemEvent = 0x04;
const int windowEvent = 0x08;
const int controllerEvent = 0x10;

// Class to recieve game events and dispatch them to subscribed parties
class EventDispatcher
{
private:
  typedef struct subscription_t {
    short subscriptionType;
    IEventSubscriber *subscriber;
  } subscription;

  EventDispatcher(const EventDispatcher &) {}
  EventDispatcher &operator=(const EventDispatcher &) { return *this; }
  SDL_Event *copyEvent(SDL_Event *event);

  static bool instantiated;
  std::vector<subscription> subscribers;
  std::vector<SDL_Event> eventQueue;
  VideoManager *videoManager;
  int queueDelayCount;

public:
  EventDispatcher(VideoManager *videoManager, int queueDelay = 0);
  ~EventDispatcher();
  void subscribe(IEventSubscriber *subscriber_in, int subscriptionMask);
  void unsubscribe(IEventSubscriber *subscriber_in);
  void enqueue(SDL_Event event);
  void flushQueue();
  bool hasEvents();
  void getEvents();
};

} // namespace CapEngine
#endif
