#ifndef EVENTSUBSCRIBER_H
#define EVENTSUBSCRIBER_H

#include "IEventSubscriber.h"
#include "gameevent.h"

#include <boost/signals2.hpp>
#include <optional>

namespace CapEngine
{

// forward declaration
class EventDispatcher;

namespace UI
{
class Widget;
}

class EventSubscriber : public IEventSubscriber
{
  public:
    EventSubscriber(std::optional<EventDispatcher *> eventDispatcher);

    void receiveEvent(SDL_Event event, Time *time) override;

    // signals
    //! The keyboard event signal
    boost::signals2::signal<void(SDL_KeyboardEvent)> m_keyboardEventSignal;
    //! The mouse motion event signal
    boost::signals2::signal<void(SDL_MouseMotionEvent)>
        m_mouseMotionEventSignal;
    //! The mouse button event signal
    boost::signals2::signal<void(SDL_MouseButtonEvent)>
        m_mouseButtonEventSignal;
    //! The mouse wheel event signal
    boost::signals2::signal<void(SDL_MouseWheelEvent)> m_mouseWheelEventSignal;
    //! The window event signal
    boost::signals2::signal<void(SDL_WindowEvent)> m_windowEventSignal;
    //! a text edit event
    boost::signals2::signal<void(SDL_TextInputEvent)> m_textInputEventSignal;
    //! A game event signal
    boost::signals2::signal<void(GameEvent &)> m_gameEventSignal;
};
} // namespace CapEngine

#endif // EVENTSUBSCRIBER_H
