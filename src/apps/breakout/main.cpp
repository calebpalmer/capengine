#include <capengine/CapEngineException.h>
#include <capengine/VideoManager.h>
#include <capengine/filesystem.h>
#include <capengine/game_management.h>
#include <capengine/locator.h>
#include <capengine/logging.h>
#include <capengine/utils.h>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/exception.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>

#include "constants.h"
#include "maingamestate.h"

int main(int argc, char* argv[])
{
    try {
        // set up the window
        const bool isFullScreen = false;

        CapEngine::WindowParams windowParams{.windowName = "breakout",
                                             .width = Breakout::kWindowWidth,
                                             .height = Breakout::kWindowHeight,
                                             .pDepth = 32,
                                             .fullScreen = isFullScreen,
                                             .opengl = false,
                                             .resizable = false,
                                             .maximized = false,
                                             .name = "breakout",
                                             .bordered = true};

        unsigned int windowId = CapEngine::init(windowParams);
        if (windowId == 0) {
            std::cout << "Failed to initialize\n";
            return 1;
        }
        CapEngine::Locator::getVideoManager().setWindowLogicalResolution(windowId, Breakout::kLogicalWindowWidth,
                                                                         Breakout::kLogicalWindowHeight);

        CapEngine::setDefaultQuitEvents();

        // load assets
        std::optional<std::string> assetFolder = CapEngine::getEnv("CP_ASSETFOLDER");
        if (!assetFolder.has_value()) {
            assetFolder =
                std::filesystem::path{CapEngine::getCurrentExecutablePath()}.parent_path().parent_path() / "resources";
        }
        BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Loading assets from folder " << *assetFolder;
        CapEngine::loadAssetFile(std::nullopt, assetFolder);

        // load custom components

        auto mainGameState = std::make_shared<Breakout::MainGameState>(windowId);

        // start loop with the gamestate object
        CapEngine::startLoop(mainGameState);
    }

    catch (CapEngine::CapEngineException const& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        std::cerr << "Stacktrace:\n" << e.trace() << "\n";
    }

    catch (boost::exception const& e) {
        std::cerr << boost::diagnostic_information(e);
        std::cout << std::stacktrace::current() << std::endl;
    }

    catch (std::exception const& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        std::cout << std::stacktrace::current() << std::endl;
    }
}
