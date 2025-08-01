#include "playerphysicscomponent.h"

#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>
#include <capengine/vector.h>

#include <optional>

#include "constants.h"
#include "gameevents.h"

namespace FlappyPei {

PlayerPhysicsComponent::PlayerPhysicsComponent()
{
    // using std::forward here doesn't really matter since handleGameEvent
    // only takes an l-value. Since std::forward preserves the l/r-value ness
    // of the call it is only really beneficial if there was a handleGameEvent force
    // both l and r-value semantics.
    CapEngine::Locator::getEventSubscriber().m_gameEventSignal.connect(
        [this](auto&& event) { handleGameEvent(std::forward<decltype(event)>(event)); });
}

/**
 * \brief Update the physics component.
 * \param object The GameObject to update.
 * \param timestep The time elapsed since the last update.
 */
void PlayerPhysicsComponent::update(CapEngine::GameObject& object, double timestep)
{
    if (object.getObjectState() != CapEngine::GameObject::ObjectState::Active) {
        // not active so reset state to defaults and return
        m_jump = false;
        return;
    }

    // if jumping, set velocity to jump velocity
    if (m_jump) {
        object.setVelocity(CapEngine::Vector{0.0, kJumpVelocity});
        m_jump = false;
    }
    // otherwise apply gravity to current velocity
    else {
        auto appliedGravity = CapEngine::Vector{0.0, kGravity} * (timestep / 1000.0);
        object.setVelocity(object.getVelocity() + appliedGravity);  // Apply gravity
    }

    auto appliedVelocity = (object.getVelocity() * (timestep / 1000.0));
    object.setPosition(object.getPosition() + appliedVelocity);  // Apply velocity
}

/**
 * \brief Gets the bounding polygon for the object.
 * \param object The object.
 * \return The bounding polygon.
 */
std::optional<CapEngine::Rectangle> PlayerPhysicsComponent::boundingPolygon(const CapEngine::GameObject& object) const
{
    CapEngine::Vector const& position = object.getPosition();
    return CapEngine::Rectangle{static_cast<int>(position.getX()), static_cast<int>(position.getY()), kSpriteWidth,
                                kSpriteHeight};
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
    try {
        auto&& playerInputEvent = dynamic_cast<PlayerInputEvent const&>(in_event);
        if (playerInputEvent.inputType == PlayerInputEvent::PlayerInputType::Jump) {
            m_jump = true;
        }
    }

    catch (const std::bad_cast& e) {
    }
}

}  // namespace FlappyPei
