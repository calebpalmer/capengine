#ifndef GAME_MANAGEMENT_H
#define GAME_MANAGEMENT_H

#include "VideoManager.h"
#include "gamestate.h"
#include "windowwidget.h"

#include <memory>

namespace CapEngine {
  extern bool initted;

  Uint32 init(WindowParams windowParams, bool noWindow=false);
  void destroy();
  void loadAssetFile(std::string assetsFile);
  void startLoop(std::shared_ptr<GameState> pGameState);
  void end();

	// state management
	void switchState(std::shared_ptr<GameState> pGameState);
  void popState();
  void pushState(std::shared_ptr<GameState> pGameState);
	std::shared_ptr<GameState> peekState();

	std::shared_ptr<UI::WindowWidget> createWindow(const std::string &name, int width, int height, bool resizable=true);

	void setDefaultQuitEvents(bool enabled=true);
}

#endif
