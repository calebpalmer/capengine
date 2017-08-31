#include "EventDispatcher.h"
#include "CapEngineException.h"
#include "VideoManager.h"
#include <memory>
#include <cassert>

using namespace CapEngine;
using namespace std;

bool EventDispatcher::instantiated = false;

EventDispatcher::EventDispatcher(VideoManager* videoManagerIn, int queuedelay) : queueDelayCount(queuedelay) {
  assert(instantiated == false);
  instantiated = true;
  assert(videoManagerIn != nullptr);
  videoManager = videoManagerIn;
}

EventDispatcher::~EventDispatcher(){ }

void EventDispatcher::subscribe(IEventSubscriber* subscriber_in, int subscriptionMask){
  if(subscriber_in == NULL){
    throw CapEngineException("Error registering event subscriber:  Event subcriber is null");
  }

  if(subscriptionMask == 0x00){
    return;
  }
  
  subscription newSubscription;
  newSubscription.subscriber = subscriber_in;
  newSubscription.subscriptionType = subscriptionMask;
  subscribers.push_back(newSubscription);
}

void EventDispatcher::enqueue(SDL_Event event){
  eventQueue.push_back(event);
}

void EventDispatcher::flushQueue(){
  vector<subscription>::iterator subscriberIter = subscribers.begin();
  vector<SDL_Event>::iterator eventIter = eventQueue.begin();
  while(eventIter != eventQueue.end()){
    SDL_Event curEvent = *eventIter;

    // Call the reshape function if SDL is being used
    if(curEvent.type == SDL_WINDOWEVENT_RESIZED){
      int w = ((SDL_WindowEvent*)&curEvent)->data1;
      int h = ((SDL_WindowEvent*)&curEvent)->data2;
      videoManager->callReshapeFunc(w, h); 
    }
    while(subscriberIter != subscribers.end()){
      subscription* curSubscription = &(*subscriberIter);
      // check to see if subscriber subscribes to current event types
      switch(curEvent.type){
      case SDL_KEYDOWN:
	if(curSubscription->subscriptionType & keyboardEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_KEYUP:
	if(curSubscription->subscriptionType & keyboardEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_MOUSEMOTION:
	if(curSubscription->subscriptionType & mouseEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_MOUSEBUTTONDOWN:
	if(curSubscription->subscriptionType & mouseEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_MOUSEBUTTONUP:
	if(curSubscription->subscriptionType & mouseEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_QUIT:
	if(curSubscription->subscriptionType & systemEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;
      case SDL_WINDOWEVENT:
	  if(curSubscription->subscriptionType & windowEvent){
	    // TODO when multiple window support is added, a window identifier should be passed along to the receiver
	    (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	  }
	break;
      case SDL_CONTROLLERAXISMOTION:
      case SDL_CONTROLLERBUTTONDOWN:
	if(curSubscription->subscriptionType & controllerEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;

      case SDL_CONTROLLERBUTTONUP:
      case SDL_CONTROLLERDEVICEADDED:
      case SDL_CONTROLLERDEVICEREMOVED:
      case SDL_CONTROLLERDEVICEREMAPPED:
	if(curSubscription->subscriptionType & controllerEvent){
	  (curSubscription->subscriber)->receiveEvent(curEvent, nullptr);
	}
	break;

      }
      subscriberIter++;
    }
    //delete *eventIter;
    eventIter++;
  }
  //eventQueue.erase(eventQueue.begin(), eventQueue.end());
  eventQueue.clear();
}

bool EventDispatcher::hasEvents(){
  return eventQueue.size() > 0;
}

void EventDispatcher::getEvents(){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    enqueue(event);
  }
}

SDL_Event* EventDispatcher::copyEvent(SDL_Event* event){
  unique_ptr<SDL_Event> copy(new SDL_Event);
  memcpy(reinterpret_cast<void*>(copy.get()), reinterpret_cast<void*>(event), sizeof(SDL_Event));
  return copy.release();
}

void EventDispatcher::unsubscribe(IEventSubscriber* subscriber_in){
  auto i = subscribers.begin();
  while(i != subscribers.end()){
    if(i->subscriber == subscriber_in){
      i = subscribers.erase(i);
    }
    else{
      i++;
    }
  }
}
