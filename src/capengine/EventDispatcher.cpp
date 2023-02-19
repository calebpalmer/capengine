#include "EventDispatcher.h"
#include "CapEngineException.h"
#include "VideoManager.h"

#include <cassert>
#include <iostream>
#include <memory>

using namespace CapEngine;
using namespace std;

bool EventDispatcher::instantiated = false;

EventDispatcher::EventDispatcher(VideoManager *videoManagerIn, int queuedelay)
    : queueDelayCount(queuedelay)
{
  assert(instantiated == false);
  instantiated = true;
  assert(videoManagerIn != nullptr);
  videoManager = videoManagerIn;
}

EventDispatcher::~EventDispatcher() {}

void EventDispatcher::subscribe(IEventSubscriber *subscriber_in,
                                int subscriptionMask)
{
  if (subscriber_in == NULL) {
    throw CapEngineException(
        "Error registering event subscriber:  Event subcriber is null");
  }

  if (subscriptionMask == 0x00) {
    return;
  }

  subscription newSubscription;
  newSubscription.subscriber = subscriber_in;
  newSubscription.subscriptionType = subscriptionMask;
  subscribers.push_back(newSubscription);
}

void EventDispatcher::enqueue(SDL_Event event) { eventQueue.push_back(event); }

void EventDispatcher::flushQueue()
{
  for (auto &&event : eventQueue) {

    // Call the reshape function if SDL is being used
    if (event.type == SDL_WINDOWEVENT_RESIZED) {
      int w = ((SDL_WindowEvent *)&event)->data1;
      int h = ((SDL_WindowEvent *)&event)->data2;
      videoManager->callReshapeFunc(w, h);
    }

    for (auto &&subscriber : subscribers) {
      // check to see if subscriber subscribes to current event types
      switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        if (subscriber.subscriptionType & keyboardEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_TEXTINPUT:
        if (subscriber.subscriptionType & keyboardEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_TEXTEDITING:
        if (subscriber.subscriptionType & keyboardEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_MOUSEMOTION:
        if (subscriber.subscriptionType & mouseEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (subscriber.subscriptionType & mouseEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (subscriber.subscriptionType & mouseEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_MOUSEWHEEL:
        if (subscriber.subscriptionType & mouseEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_QUIT:
        if (subscriber.subscriptionType & systemEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_WINDOWEVENT:
        if (subscriber.subscriptionType & windowEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      case SDL_CONTROLLERAXISMOTION:
      case SDL_CONTROLLERBUTTONDOWN:
        if (subscriber.subscriptionType & controllerEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;

      case SDL_CONTROLLERBUTTONUP:
      case SDL_CONTROLLERDEVICEADDED:
      case SDL_CONTROLLERDEVICEREMOVED:
      case SDL_CONTROLLERDEVICEREMAPPED:
        if (subscriber.subscriptionType & controllerEvent) {
          (subscriber.subscriber)->receiveEvent(event, nullptr);
        }
        break;
      }
    }
  }
  eventQueue.clear();
}

bool EventDispatcher::hasEvents() { return eventQueue.size() > 0; }

void EventDispatcher::getEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    enqueue(event);
  }
}

SDL_Event *EventDispatcher::copyEvent(SDL_Event *event)
{
  unique_ptr<SDL_Event> copy(new SDL_Event);
  memcpy(reinterpret_cast<void *>(copy.get()), reinterpret_cast<void *>(event),
         sizeof(SDL_Event));
  return copy.release();
}

void EventDispatcher::unsubscribe(IEventSubscriber *subscriber_in)
{
  auto i = subscribers.begin();
  while (i != subscribers.end()) {
    if (i->subscriber == subscriber_in) {
      i = subscribers.erase(i);
    } else {
      i++;
    }
  }
}
