#ifndef GAME_MANAGEMENT_H
#define GAME_MANAGEMENT_H

#include "VideoManager.h"
#include "gamestate.h"

#include <memory>

namespace CapEngine {
  extern bool initted;

  void init(Screen_t screenConfig);
  void destroy();
  void loadAssetFile(std::string assetsFile);
  void startLoop(std::unique_ptr<GameState> pGameState);
  void end();
}

#endif
