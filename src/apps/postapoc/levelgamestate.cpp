#include "levelgamestate.h"

#include <capengine/collision.h>
#include <capengine/locator.h>
#include <capengine/logger.h>
#include <capengine/logging.h>
#include <capengine/tiledmap.h>

#include <algorithm>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <memory>

namespace PA {

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

    // 339, 176
    m_camera.center(CapEngine::Rectangle{339, 176, 1, 1},
                    CapEngine::Rectangle{0, 0, static_cast<double>(m_map->pixelWidth()),
                                         static_cast<double>(m_map->pixelHeight())});
}

void LevelGameState::update(double in_ms)
{
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

    videoManager.drawTexture(m_windowId, dstRect, m_map->texture(), srcRect);
}

}  // namespace PA
