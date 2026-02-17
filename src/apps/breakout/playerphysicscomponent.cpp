#include "playerphysicscomponent.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>
#include <capengine/logging.h>
#include <capengine/pcm.h>
#include <capengine/vector.h>

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <gsl/gsl-lite.hpp>
#include <memory>
#include <optional>

#include "constants.h"
// #include "gameevents.h"

namespace Breakout {

PlayerPhysicsComponent::PlayerPhysicsComponent()
{
    // using std::forward here doesn't really matter since handleGameEvent
    // only takes an l-value. Since std::forward preserves the l/r-value ness
    // of the call it is only really beneficial if there was a handleGameEvent force
    // both l and r-value semantics.
    // CapEngine::Locator::getEventSubscriber().m_gameEventSignal.connect(
    //     [this](auto&& event) { handleGameEvent(std::forward<decltype(event)>(event)); });
}

/**
 * \brief Update the physics component.
 * \param object The GameObject to update.
 * \param timestep The time elapsed since the last update.
 */
void PlayerPhysicsComponent::update(CapEngine::GameObject& object, double timestep)
{
    if (object.getObjectState() == CapEngine::GameObject::ObjectState::Active) {
        const uint8_t* kbState = SDL_GetKeyboardState(nullptr);
        CAP_THROW_NULL(kbState, "Keyboard state is null");

        const bool moveLeft = static_cast<bool>(kbState[SDL_SCANCODE_LEFT]);
        const bool moveRight = static_cast<bool>(kbState[SDL_SCANCODE_RIGHT]);

        if (moveLeft && moveRight) {
            // what are you doing dummy? Just return.
            return;
        }

        int velocity = 0;
        if (moveLeft) {
            velocity = (-1) * kPaddleVelocity;
        }
        if (moveRight) {
            velocity = kPaddleVelocity;
        }

        CapEngine::Vector appliedVelocity{velocity * (timestep / 1000.0)};
        object.setPosition(object.getPosition() + appliedVelocity);  // Apply velocity

        return;
    }
}

/**
 * \brief Gets the bounding polygon for the object.
 * \param object The object.
 * \return The bounding polygon.
 */
std::optional<CapEngine::Rectangle> PlayerPhysicsComponent::boundingPolygon(const CapEngine::GameObject& object) const
{
    CapEngine::Vector const& position = object.getPosition();
    return CapEngine::Rectangle{position.getX(), position.getY(), kPaddleWidth, kPaddleHeight};
}

/**
 * \brief Clones the component.
 * \return A unique pointer to the new component.
 */
std::unique_ptr<CapEngine::Component> PlayerPhysicsComponent::clone() const
{
    return std::make_unique<PlayerPhysicsComponent>(*this);
}

void PlayerPhysicsComponent::handleGameEvent(CapEngine::GameEvent const& in_event)
{
    // try {
    //     auto&& playerInputEvent = dynamic_cast<PlayerInputEvent const&>(in_event);
    //     if (playerInputEvent.inputType == PlayerInputEvent::PlayerInputType::Jump) {
    //         m_jump = true;
    //     }
    // }

    // catch (const std::bad_cast& e) {
    // }
}

}  // namespace Breakout
