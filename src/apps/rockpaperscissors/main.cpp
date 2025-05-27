#include <capengine/game_management.h>
#include <capengine/locator.h>
#include <capengine/filesystem.h>
#include <capengine/utils.h>
#include <capengine/logging.h>

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <cstdlib>

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
    std::optional<std::string> assetFolder = CapEngine::getEnv("CP_ASSETFOLDER");
    if(!assetFolder.has_value()){
        assetFolder = std::filesystem::path{CapEngine::getCurrentExecutablePath()}.parent_path().parent_path() / "resources";
    }
    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Loading assets from folder " << *assetFolder;
    CapEngine::loadAssetFile(std::nullopt, assetFolder);

    // load custom components

    // create gamestate object
    auto rpsState = std::make_shared<RPS::RockPaperScissorsState>(windowId);

    // start loop with the gamestate object
    CapEngine::startLoop(rpsState);
}
