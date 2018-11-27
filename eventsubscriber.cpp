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
void EventSubscriber::receiveEvent(SDL_Event event, Time* /*time*/){
	switch (event.type){
	case SDL_WINDOWEVENT:
		m_windowEventSignal(event.window);
		break;

	case SDL_KEYUP:
	case SDL_KEYDOWN:
		m_keyboardEventSignal(event.key);
		break;

	case SDL_TEXTINPUT:
		m_textInputEventSignal(event.text);
		break;

	case SDL_MOUSEMOTION:
		m_mouseMotionEventSignal(event.motion);
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		m_mouseButtonEventSignal(event.button);
		break;

	case SDL_MOUSEWHEEL:
		m_mouseWheelEventSignal(event.wheel);
		break;
	}
}

} // namespace CapEngine
