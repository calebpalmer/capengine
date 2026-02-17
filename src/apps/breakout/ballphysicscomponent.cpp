#include "ballphysicscomponent.h"

#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/vector.h>

#include <optional>

namespace Breakout {

BallPhysicsComponent::BallPhysicsComponent(int in_ballDiameter) : m_ballDiameter(in_ballDiameter)
{
}

void BallPhysicsComponent::update(CapEngine::GameObject& object, double timestep)
{
    // get the objects acceleration and velocity and translate the velocity
    CapEngine::Vector acceleration = object.getAcceleration();
    CapEngine::Vector velocity = object.getVelocity();
    velocity = velocity + (acceleration * (timestep / 1000.0));
    object.setVelocity(velocity);

    CapEngine::Vector position = object.getPosition();
    position = position + (velocity * (timestep / 1000.0));
    object.setPosition(position);
}

std::optional<CapEngine::Rectangle> BallPhysicsComponent::boundingPolygon(const CapEngine::GameObject& object) const
{
    CapEngine::Vector position = object.getPosition();
    return CapEngine::Rectangle{position.getX(), position.getY(), m_ballDiameter, m_ballDiameter};
}

std::unique_ptr<CapEngine::Component> BallPhysicsComponent::clone() const
{
    return std::make_unique<BallPhysicsComponent>(*this);
}

void BallPhysicsComponent::handleGameEvent(const CapEngine::GameEvent& in_event)
{
}

}  // namespace Breakout
