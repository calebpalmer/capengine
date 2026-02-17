#include "catphysicscomponent.h"

#include <capengine/collision.h>
#include <capengine/gameobject.h>

#include <memory>

#include "constants.h"

namespace FlappyPei {

CatPhysicsComponent::CatPhysicsComponent(int in_gapLocation, int in_gapSize)
    : m_gapLocation(in_gapLocation),
      m_gapSize(in_gapSize),
      m_topMbr(0, 0, kCatWidth, in_gapLocation),
      m_bottomMbr(0, in_gapLocation + in_gapSize, kCatWidth, kLogicalWindowHeight - (in_gapLocation + in_gapSize))
{
}

void CatPhysicsComponent::update(CapEngine::GameObject& object, double timestep)
{
    if (object.getObjectState() != CapEngine::GameObject::ObjectState::Active) {
        return;
    }

    auto appliedVelocity = (object.getVelocity() * (timestep / 1000.0));
    object.setPosition(object.getPosition() + appliedVelocity);  // Apply velocity

    // update MBRs
    m_topMbr.x = object.getPosition().getX();
    m_bottomMbr.x = m_topMbr.x;

    // check to see if cat is out of bounds
    if (m_topMbr.x + m_topMbr.width < 0) {
        object.setObjectState(CapEngine::GameObject::Dead);
    }
}

std::optional<CapEngine::Rectangle> CatPhysicsComponent::boundingPolygon(const CapEngine::GameObject& object) const
{
    // todo
    return CapEngine::Rectangle{object.getPosition().getX(), object.getPosition().getY(), m_gapSize,
                                kLogicalWindowHeight};
}

std::unique_ptr<CapEngine::Component> CatPhysicsComponent::clone() const
{
    return std::make_unique<CatPhysicsComponent>(*this);
}

int CatPhysicsComponent::gapLocation() const
{
    return m_gapLocation;
}

double CatPhysicsComponent::gapSize() const
{
    return m_gapSize;
}

CapEngine::CollisionType CatPhysicsComponent::collides([[maybe_unused]] CapEngine::Rectangle const& in_mbr) const
{
    // check top
    CapEngine::CollisionType collision = CapEngine::detectMBRCollision(m_topMbr, in_mbr);
    if (collision != CapEngine::COLLISION_NONE) {
        return collision;
    }

    // check bottom
    return CapEngine::detectMBRCollision(m_bottomMbr, in_mbr);
}

}  // namespace FlappyPei
