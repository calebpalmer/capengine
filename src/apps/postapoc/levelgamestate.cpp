#include "levelgamestate.h"

#include <apps/postapoc/constants.h>
#include <apps/postapoc/inputcontroller.h>
#include <apps/postapoc/playergraphicscomponent.h>
#include <apps/postapoc/playerinputcomponent.h>
#include <apps/postapoc/playerphysicscomponent.h>
#include <capengine/CapEngineException.h>
#include <capengine/collision.h>
#include <capengine/locator.h>
#include <capengine/logger.h>
#include <capengine/logging.h>
#include <capengine/tiledmap.h>
#include <capengine/gameobject.h>
#include <capengine/boxcollider.h>

#include <optional>
#include <string_view>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <memory>

namespace PA {

namespace {

const std::string_view kObjectsObjectGroupName{"objects"};
const std::string_view kPlayerObjectGroupObjectName{"playerStart"};

std::unique_ptr<CapEngine::GameObject> makePlayer(const CapEngine::TiledMap& in_map)
{
    auto playerObject = std::make_unique<CapEngine::GameObject>();
    playerObject->addComponent(std::make_shared<PlayerGraphicsComponent>());
    playerObject->addComponent(std::make_shared<PlayerInputComponent>());
    playerObject->addComponent(std::make_shared<PlayerPhysicsComponent>());
    playerObject->addComponent(
        std::make_shared<CapEngine::BoxCollider>(CapEngine::Rectangle{0.0, 0.0, kPlayerWidth, kPlayerHeight}));

    auto objectGroup = in_map.objectGroupByName(kObjectsObjectGroupName);
    CAP_THROW_ASSERT(objectGroup.has_value(), "objects object layer missing from map");

    auto playerPositionObject = objectGroup->get().objectByName(kPlayerObjectGroupObjectName);
    CAP_THROW_ASSERT(playerPositionObject.has_value(), "player start missing from map");

    // position from tiled is bottom center and its y-down
    CapEngine::Vector initialPosition{playerPositionObject->x - (kPlayerWidth / 2.0),
                                      objectGroup->get().tiledToWorldY(playerPositionObject->y)};
    playerObject->setPosition(initialPosition);  // Set initial position
    playerObject->setObjectState(CapEngine::GameObject::ObjectState::Starting);

    return playerObject;
}
}  // namespace

//! Constructor
LevelGameState::LevelGameState(uint32_t in_windowId, std::string in_mapPath) : m_windowId(in_windowId), m_camera(0, 0)
{
    std::filesystem::path mapPath{in_mapPath};
    if (!std::filesystem::exists(mapPath) && mapPath.is_relative()) {
        // try it with the basepath from assetmanager
        auto basePath = CapEngine::Locator::getAssetManager().getBasePath();
        if (basePath) {
            mapPath = *basePath / mapPath;
        }
    }

    m_map = std::make_unique<CapEngine::TiledMap>(mapPath);

    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Writing rendered map to /tmp/map.png";
    m_map->render();
    CapEngine::Locator::getVideoManager().saveTexture(m_map->texture(), "/tmp/map.png");

    // initialize the camera
    auto& videoManager = CapEngine::Locator::getVideoManager();
    auto [logicalWidth, logicalHeight] = videoManager.getWindowLogicalResolution(m_windowId);
    m_camera.setWidth(logicalWidth);
    m_camera.setHeight(logicalHeight);
    m_camera.setPosition(0, 0);

    // gameobjects
    m_player = makePlayer(*m_map);

    // events
    m_keyboardEventConnection = CapEngine::Locator::getEventSubscriber().m_keyboardEventSignal.connect(
        [this](const PA::InputEvent& event) { m_inputController.handle(event); });
    m_inputController.enabled(true);
}

void LevelGameState::update(double in_ms)
{
    // upate the objects
    assert(m_player != nullptr);
    m_player->updateInPlace(in_ms);

    // do collision detection

    // center the camera on the player
    CapEngine::Rectangle playerRect = m_player->boundingPolygon();
    CapEngine::Rectangle mapRect{0.0, 0.0, static_cast<double>(m_map->pixelWidth()),
                                 static_cast<double>(m_map->pixelHeight())};
    m_camera.center(playerRect, mapRect);
}

void LevelGameState::render()
{
    auto& videoManager = CapEngine::Locator::getVideoManager();
    auto [windowWidth, windowHeight] = videoManager.getWindowResolution(m_windowId);

    auto [logicalWidth, logicalHeight] = videoManager.getWindowLogicalResolution(m_windowId);

    // render background first
    videoManager.drawFillRect(m_windowId, CapEngine::Rect{0, 0, logicalWidth, logicalHeight},
                              CapEngine::Colour{0xA0, 0xA0, 0xA0, 0xFF});

    // this only renders to a map owned texture
    assert(m_map != nullptr);
    m_map->render();

    // render the map texture to the window
    CapEngine::Rect dstRect{0, 0, logicalWidth, logicalHeight};
    CapEngine::Rect srcRect = m_camera.getViewingRectangle().toRect();

    CAP_THROW_ASSERT(srcRect.w == dstRect.w);
    CAP_THROW_ASSERT(srcRect.h == dstRect.h);

    videoManager.drawTexture(m_windowId, dstRect, m_map->texture(), srcRect);

    assert(m_player != nullptr);
    m_player->render(m_camera, m_windowId);
}

}  // namespace PA
