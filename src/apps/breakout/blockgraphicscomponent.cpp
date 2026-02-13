#include "blockgraphicscomponent.h"

#include <capengine/collision.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>

#include <memory>

namespace Breakout {

BlockGraphicsComponent::BlockGraphicsComponent(int in_width, int in_height, CapEngine::Colour in_colour)
    : m_width(in_width), m_height(in_height), m_colour(in_colour)
{
}

void BlockGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

void BlockGraphicsComponent::render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera,
                                    uint32_t in_windowId)
{
    CapEngine::Vector position = object.getPosition();
    CapEngine::Rect rect{
        .x = static_cast<int>(position.getX()), .y = static_cast<int>(position.getY()), .w = m_width, .h = m_height};
    CapEngine::Locator::getVideoManager().drawFillRect(in_windowId, rect, m_colour);
}

std::unique_ptr<CapEngine::Component> BlockGraphicsComponent::clone() const
{
    return std::make_unique<BlockGraphicsComponent>(*this);
}

}  // namespace Breakout
