#ifndef POSTAPOC_LEVELGAMESTATE_H
#define POSTAPOC_LEVELGAMESTATE_H

#include "inputcontroller.h"

#include <capengine/gameobject.h>
#include <capengine/gamestate.h>
#include <capengine/tiledmap.h>
#include <capengine/camera2d.h>

#include <boost/signals2/connection.hpp>
#include <memory>
#include <cstdint>
#include <string>

namespace PA {

class LevelGameState final : public CapEngine::GameState {
   public:
    LevelGameState(uint32_t in_windowId, std::string in_mapPath);
    ~LevelGameState() override = default;

    void render() override;
    void update(double ms) override;

   private:
    uint32_t m_windowId = 0;                     //!< ID of the window to render to
    std::unique_ptr<CapEngine::TiledMap> m_map;  //!< Tiled map containing game layout and assets
    CapEngine::Camera2d m_camera;
    std::unique_ptr<CapEngine::GameObject> m_player;
    InputController m_inputController;

    boost::signals2::scoped_connection m_keyboardEventConnection;
};

}  // namespace PA

#endif /* POSTAPOC_LEVELGAMESTATE_H */
