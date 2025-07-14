#include "playerphysicscomponent.h"

#include <capengine/collision.h>
#include <capengine/gameobject.h>

#include <optional>

#include "constants.h"

namespace FlappyPei {

std::optional<CapEngine::Rectangle> PlayerPhysicsComponent::boundingPolygon(const CapEngine::GameObject& object) const
{
    CapEngine::Vector position = object.getPosition();
    return CapEngine::Rectangle{static_cast<int>(position.getX()), static_cast<int>(position.getY()), kSpriteWidth,
                                kSpriteHeight};
}

std::unique_ptr<CapEngine::Component> PlayerPhysicsComponent::clone() const
{
    return std::make_unique<PlayerPhysicsComponent>(*this);
}

}  // namespace FlappyPei
