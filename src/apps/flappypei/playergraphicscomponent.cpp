#include "playergraphicscomponent.h"

#include <capengine/colour.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>
#include <capengine/vector.h>

#include "constants.h"

namespace FlappyPei {

/**
 * \brief Renders the player object.
 * \param object The GameObject to render.
 * \param in_camera The camera used for rendering.
 * \param in_windowId The ID of the window to render to.
 */
void PlayerGraphicsComponent::render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera,
                                     uint32_t in_windowId)
{
    CapEngine::Rect position{.x = static_cast<int>(object.getPosition().getX()),
                             .y = static_cast<int>(object.getPosition().getY()),
                             .w = kSpriteWidth,
                             .h = kSpriteHeight};
    CapEngine::Colour fillColour{139, 69, 19, 255};
    CapEngine::Locator::getVideoManager().drawFillRect(in_windowId, position, fillColour);
}

/**
 * \brief Updates the graphics component.
 * \param object The GameObject to update.
 * \param timestep The time elapsed since the last update.
 */
void PlayerGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

/**
 * \brief Clones the component.
 * \return A unique pointer to the new component.
 */
std::unique_ptr<CapEngine::Component> PlayerGraphicsComponent::clone() const
{
    return std::make_unique<PlayerGraphicsComponent>(*this);
}

}  // namespace FlappyPei
