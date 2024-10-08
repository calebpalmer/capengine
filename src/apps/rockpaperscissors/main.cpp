#include <capengine/game_management.h>
#include <capengine/locator.h>

#include <iostream>
#include <memory>
#include <string>

#include "rockpaperscissorsstate.h"

int main(int argc, char *argv[])
{
    // set up the window
    const bool isFullScreen = false;

    CapEngine::WindowParams windowParams{
        "tanks", 1600,         900,
        32,      isFullScreen, false,
        true,    false,        "rockpaperscissors",
        true};

    unsigned int windowId = CapEngine::init(windowParams);
    if (windowId == 0) {
        std::cout << "Failed to initialize\n";
        return 1;
    }

    CapEngine::setDefaultQuitEvents();

    // load assets

    // load custom components

    // create gamestate object
    auto rpsState = std::make_shared<RPS::RockPaperScissorsState>(windowId);

    // start loop with the gamestate object
    CapEngine::startLoop(rpsState);
}
