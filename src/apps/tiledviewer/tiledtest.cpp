#include "teststate.h"

#include <capengine/game_management.h>
#include <capengine/locator.h>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Not enough arguments\n";
        return 1;
    }

    std::string mapFile{argv[1]};
    std::cout << "mapFile: " << mapFile << "\n";

    // set up the window
    const bool isFullScreen = false;

    CapEngine::WindowParams windowParams{"tanks", 1280, 720, 32, isFullScreen, false, false, false, "main"};

    unsigned int windowId = CapEngine::init(windowParams);
    CapEngine::Locator::videoManager->setWindowLogicalResolution(windowId, 1280, 720);

    CapEngine::setDefaultQuitEvents();

    // load assets

    // load custom components

    // create gamestate object
    auto testState = std::make_shared<Game::TestState>(std::move(mapFile));

    // start loop with the gamestate object
    CapEngine::startLoop(testState);
}
