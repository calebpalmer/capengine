#include "playerinputcomponent.h"

#include <apps/postapoc/gameevents.h>
#include <capengine/locator.h>
#include <capengine/logging.h>

#include <boost/log/trivial.hpp>

namespace PA {

PlayerInputComponent::PlayerInputComponent()
{
    m_gameEventConnection = CapEngine::Locator::getEventSubscriber().m_gameEventSignal.connect(
        [this](auto&& in_event) { this->handle(in_event); });
}

void PlayerInputComponent::handle(const CapEngine::GameEvent& in_event)
{
    const auto* playerInputEvent = dynamic_cast<const PlayerInputEvent*>(&in_event);
    if (playerInputEvent != nullptr) {
        if (playerInputEvent->inputType == PlayerInputEvent::PlayerInputType::MoveLeft) {
            m_walkLeft = playerInputEvent->pressed;
        }
        if (playerInputEvent->inputType == PlayerInputEvent::PlayerInputType::MoveRight) {
            m_walkRight = playerInputEvent->pressed;
        }
        if (playerInputEvent->inputType == PlayerInputEvent::PlayerInputType::MoveUp) {
            m_walkUp = playerInputEvent->pressed;
        }
        if (playerInputEvent->inputType == PlayerInputEvent::PlayerInputType::MoveDown) {
            m_walkDown = playerInputEvent->pressed;
        }
    }
}

void PlayerInputComponent::update(CapEngine::GameObject& in_object, double /*timestep*/)
{
}

}  // namespace PA
