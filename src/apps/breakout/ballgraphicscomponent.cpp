#include "ballgraphicscomponent.h"

#include <capengine/VideoManager.h>
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
    CapEngine::Rectangle rect{position.getX(), position.getY(), static_cast<double>(m_diameter),
                              static_cast<double>(m_diameter)};

    CapEngine::VideoManager& videoManager = CapEngine::Locator::getVideoManager();
    videoManager.drawFillRect(in_windowId, rect.toRect(), m_colour);
}

void BallGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

}  // namespace Breakout
