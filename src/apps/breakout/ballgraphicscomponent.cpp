#include "ballgraphicscomponent.h"

#include <capengine/gameobject.h>
#include <capengine/locator.h>

namespace Breakout {

BallGraphicsComponent::BallGraphicsComponent(int in_diameter, CapEngine::Colour in_colour)
    : m_diameter(in_diameter), m_colour(in_colour)
{
}

void BallGraphicsComponent::render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera,
                                   uint32_t in_windowId)
{
    CapEngine::Vector position = object.getPosition();
    CapEngine::Rect rect{.x = static_cast<int>(position.getX()),
                         .y = static_cast<int>(position.getY()),
                         .w = m_diameter,
                         .h = m_diameter};
    CapEngine::Locator::getVideoManager().drawFillRect(in_windowId, rect, m_colour);
}

void BallGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

std::unique_ptr<CapEngine::Component> BallGraphicsComponent::clone() const
{
    return std::make_unique<BallGraphicsComponent>(*this);
}

}  // namespace Breakout
