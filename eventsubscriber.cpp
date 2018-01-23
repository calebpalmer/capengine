#include "eventsubscriber.h"

#include "EventDispatcher.h"
#include "widget.h"

namespace CapEngine {

//! Constructor
/**
* \param eventDispatcher The EventDispatcher to subscribe to
*/
EventSubscriber::EventSubscriber(EventDispatcher &eventDispatcher){
	int subscriptionMask = mouseEvent | keyboardEvent | systemEvent | windowEvent | controllerEvent;
	IEventSubscriber::subscribe(&eventDispatcher, subscriptionMask);
}

//! @copydoc IEventSubscriber::receiveEvent()
void EventSubscriber::receiveEvent(SDL_Event event, Time *time){
	
}

} // namespace CapEngine
