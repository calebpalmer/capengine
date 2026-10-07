#include "inputcontroller.h"

#include <SDL_events.h>
#include <apps/postapoc/gameevents.h>
#include <capengine/locator.h>

#include <SDL_keycode.h>

namespace PA {

void InputController::enabled(bool in_enabled)
{
    m_enabled = in_enabled;
}

void InputController::handle(InputEvent in_event)
{
    if (m_enabled) {
        std::visit([this](const auto& event) { this->handle(event); }, in_event);
    }
}

void InputController::handle(const SDL_KeyboardEvent& in_event)
{
    if (m_state == ControllerState::Main) {
        if (in_event.keysym.sym == SDLK_LEFT) {
            PlayerInputEvent gameEvent;
            gameEvent.inputType = PlayerInputEvent::PlayerInputType::MoveLeft;
            gameEvent.pressed = in_event.type == SDL_KEYDOWN;

            CapEngine::Locator::getEventSubscriber().m_gameEventSignal(gameEvent);
        }
        if (in_event.keysym.sym == SDLK_RIGHT) {
            PlayerInputEvent gameEvent;
            gameEvent.inputType = PlayerInputEvent::PlayerInputType::MoveRight;
            gameEvent.pressed = in_event.type == SDL_KEYDOWN;

            CapEngine::Locator::getEventSubscriber().m_gameEventSignal(gameEvent);
        }
        if (in_event.keysym.sym == SDLK_UP) {
            PlayerInputEvent gameEvent;
            gameEvent.inputType = PlayerInputEvent::PlayerInputType::MoveUp;
            gameEvent.pressed = in_event.type == SDL_KEYDOWN;

            CapEngine::Locator::getEventSubscriber().m_gameEventSignal(gameEvent);
        }
        if (in_event.keysym.sym == SDLK_DOWN) {
            PlayerInputEvent gameEvent;
            gameEvent.inputType = PlayerInputEvent::PlayerInputType::MoveDown;
            gameEvent.pressed = in_event.type == SDL_KEYDOWN;

            CapEngine::Locator::getEventSubscriber().m_gameEventSignal(gameEvent);
        }
    }
}

void InputController::handle(const SDL_MouseButtonEvent& in_event)
{
    nullptr;
}

void InputController::handle(const SDL_ControllerButtonEvent& in_event)
{
    nullptr;
}

}  // namespace PA
