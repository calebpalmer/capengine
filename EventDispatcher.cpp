#include "EventDispatcher.h"
#include "CapEngineException.h"
#include "VideoManager.h"
#include <memory>

using namespace CapEngine;
using namespace std;

unique_ptr<EventDispatcher> EventDispatcher::instance = unique_ptr<EventDispatcher>(new EventDispatcher());

EventDispatcher::EventDispatcher(int queuedelay) : queueDelayCount(queuedelay) {
  auto_ptr<vector<subscription> > subscribersPtr(new vector<subscription>());
  auto_ptr<vector<SDL_Event*> > eventQueuePtr(new vector<SDL_Event*>());
  subscribers = subscribersPtr.release();
  eventQueue = eventQueuePtr.release();
}

EventDispatcher::~EventDispatcher(){
  delete subscribers;
  delete eventQueue;
}

void EventDispatcher::subscribe(IEventSubscriber* subscriber_in, int subscriptionMask){
  if(subscriber_in == NULL){
    throw new CapEngineException("Error registering event subscriber:  Event subcriber is null");
  }

  if(subscriptionMask == 0x00){
    return;
  }
  
  subscription newSubscription;
  newSubscription.subscriber = subscriber_in;
  newSubscription.subscriptionType = subscriptionMask;
  subscribers->push_back(newSubscription);
}

void EventDispatcher::enqueue(SDL_Event* event){
  eventQueue->push_back(event);
}

void EventDispatcher::flushQueue(){
  vector<subscription>::iterator subscriberIter = subscribers->begin();
  vector<SDL_Event*>::iterator eventIter = eventQueue->begin();
  while(eventIter != eventQueue->end()){
    SDL_Event* curEvent = *eventIter;
    if(curEvent->type == SDL_VIDEORESIZE){
      int w = ((SDL_ResizeEvent*)curEvent)->h;
      int h = ((SDL_ResizeEvent*)curEvent)->h;
      VideoManager::getInstance().callReshapeFunc(w, h);
    }
    while(subscriberIter != subscribers->end()){
      subscription* curSubscription = &(*subscriberIter);
      // check to see if subscriber subscribes to current event types
      switch(curEvent->type){
      case SDL_KEYDOWN:
	if(curSubscription->subscriptionType & keyboardEvent){
	  (curSubscription->subscriber)->receiveEvent(copyEvent(curEvent), nullptr);
	}
	break;
      case SDL_KEYUP:
	if(curSubscription->subscriptionType & keyboardEvent){
	  (curSubscription->subscriber)->receiveEvent(copyEvent(curEvent), nullptr);
	}
	break;
      case SDL_MOUSEMOTION:
	if(curSubscription->subscriptionType & mouseEvent){
	  (curSubscription->subscriber)->receiveEvent(copyEvent(curEvent), nullptr);
	}
	break;
      case SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP:
	if(curSubscription->subscriptionType & mouseEvent){
	  (curSubscription->subscriber)->receiveEvent(copyEvent(curEvent), nullptr);
	}
	break;
      case SDL_QUIT:
	if(curSubscription->subscriptionType & systemEvent){
	  (curSubscription->subscriber)->receiveEvent(copyEvent( curEvent), nullptr);
	}
	break;
      }
      subscriberIter++;
    }
    //delete *eventIter;
    eventIter++;
    delete curEvent;
  }
  //eventQueue->erase(eventQueue->begin(), eventQueue->end());
  eventQueue->clear();
}

bool EventDispatcher::hasEvents(){
  if(eventQueue->size() > 0){
    return true;
  }
  else{
    return false;
  }
}

void EventDispatcher::getEvents(){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    enqueue(copyEvent(&event));
  }
}

SDL_Event* EventDispatcher::copyEvent(SDL_Event* event){
  unique_ptr<SDL_Event> copy(new SDL_Event);
  memcpy(reinterpret_cast<void*>(copy.get()), reinterpret_cast<void*>(event), sizeof(SDL_Event));
  return copy.release();
}

EventDispatcher& EventDispatcher::getInstance(){
  if(instance.get() == nullptr){
    instance.reset(new EventDispatcher());
  }
  return *(instance.get());
}
