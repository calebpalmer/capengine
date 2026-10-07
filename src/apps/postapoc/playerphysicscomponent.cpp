#include "playerphysicscomponent.h"

#include <apps/postapoc/constants.h>
#include <apps/postapoc/playerinputcomponent.h>
#include <capengine/CapEngineException.h>
#include <capengine/gameobject.h>
#include <capengine/logging.h>
#include <capengine/physics.h>

#include <boost/log/trivial.hpp>

namespace PA {

void PlayerPhysicsComponent::update(CapEngine::GameObject& in_object, double in_timestep)
{
    // translate character
    {
        auto position = in_object.getPosition();
        auto velocity = in_object.getVelocity();
        auto newPosition = CapEngine::applyDisplacement(velocity, position, in_timestep);
        in_object.setPosition(newPosition);
    }

    // Get input state and apply
    auto&& playerInputComponents = in_object.getComponents<PA::PlayerInputComponent>();
    if (playerInputComponents.empty()) {
        BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning) << "PlayerInputComponent not found.";
    }
    if (playerInputComponents.size() > 1) {
        BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning) << "More than one PlayerInputComponent found.";
    }

    auto&& playerInputComponent = playerInputComponents[0];
    CAP_THROW_NULL(playerInputComponent);

    CapEngine::Vector velocity;
    if (playerInputComponent->m_walkLeft) {
        velocity.setX(kPlayerWalkVelocity * (-1));
    }
    if (playerInputComponent->m_walkRight) {
        velocity.setX(kPlayerWalkVelocity);
    }
    if (playerInputComponent->m_walkDown) {
        velocity.setY(kPlayerWalkVelocity * (-1));
    }
    if (playerInputComponent->m_walkUp) {
        velocity.setY(kPlayerWalkVelocity);
    }

    in_object.setVelocity(velocity);
}

std::optional<CapEngine::Rectangle> PlayerPhysicsComponent::boundingPolygon(
    const CapEngine::GameObject& /*object*/) const
{
    // we have a box collider for this
    return std::nullopt;
}

}  // namespace PA
