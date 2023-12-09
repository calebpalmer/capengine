#include <capengine/game_management.h>
#include <capengine/locator.h>

#include <iostream>
#include <memory>
#include <string>

#include "tiledviewer.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cout << "Not enough arguments\n";
		return 1;
	}

	std::string mapFile{argv[1]};
	std::cout << "mapFile: " << mapFile << "\n";

	// set up the window
	const bool isFullScreen = false;

	CapEngine::WindowParams windowParams{
		"tanks", 1600, 900, 32, isFullScreen, false, true, false, "main", true};

	unsigned int windowId = CapEngine::init(windowParams);
	if (windowId == 0) {
		std::cout << "Failed to initialize\n";
		return 1;
	}

	CapEngine::setDefaultQuitEvents();

	// load assets

	// load custom components

	// create gamestate object
	auto tiledViewerState =
		std::make_shared<Game::TiledViewerState>(windowId, std::move(mapFile));

	// start loop with the gamestate object
	CapEngine::startLoop(tiledViewerState);
}
