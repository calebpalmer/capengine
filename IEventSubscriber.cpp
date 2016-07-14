#include "IEventSubscriber.h"

#include "EventDispatcher.h"

using namespace CapEngine;

void IEventSubscriber::subscribe(EventDispatcher* eventDispatcher, int subscriptionMask){
  m_pEventDispatcher = eventDispatcher;
  m_pEventDispatcher->subscribe(this, subscriptionMask);
}

IEventSubscriber::~IEventSubscriber(){
  m_pEventDispatcher->unsubscribe(this);
}

