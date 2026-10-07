#include "playergraphicscomponent.h"

#include <capengine/camera2d.h>
#include <capengine/colour.h>
#include <capengine/gameobject.h>
#include <capengine/locator.h>
#include <capengine/vector.h>

#include "constants.h"

namespace PA {

namespace {
constexpr int kSpriteWidth{16};
constexpr int kSpriteHeight{24};

constexpr int kSpriteSheetAssetId = 1000;
constexpr std::string_view kIdleFrameName{"idle"};

}  // namespace

/**
 * \brief Renders the player object.
 * \param object The GameObject to render.
 * \param in_camera The camera used for rendering.
 * \param in_windowId The ID of the window to render to.
 */
void PlayerGraphicsComponent::render(CapEngine::GameObject& object, const CapEngine::Camera2d& in_camera,
                                     uint32_t in_windowId)
{
    CapEngine::Rectangle position{object.getPosition().getX(), object.getPosition().getY(), kSpriteWidth,
                                  kSpriteHeight};
    position = CapEngine::worldToCameraCoords(in_camera, position, in_windowId);

    CapEngine::Locator::getAssetManager().drawFrame(in_windowId, kSpriteSheetAssetId, std::string{kIdleFrameName}, 0,
                                                    position);
}

/**
 * \brief Updates the graphics component.
 * \param object The GameObject to update.
 * \param timestep The time elapsed since the last update.
 */
void PlayerGraphicsComponent::update(CapEngine::GameObject& object, double timestep)
{
}

}  // namespace PA
