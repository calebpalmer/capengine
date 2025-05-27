#include "game_management.h"

#include <boost/throw_exception.hpp>
#include <memory>
#include <optional>

#include "EventDispatcher.h"
#include "VideoManager.h"
#include "bitmapcollisionlayer.h"
#include "boxcollider.h"
#include "componentfactory.h"
#include "controller.h"
#include "eventsubscriber.h"
#include "imagelayer.h"
#include "layerfactory.h"
#include "locator.h"
#include "placeholdergraphics.h"
#include "runner.h"
#include "windowwidget.h"
#include "logging.h"

namespace
{

constexpr std::string_view kAssetFileProperty = "CapengineAssetPath";

}

namespace CapEngine
{

bool initted = false;

Uint32 init(WindowParams screenConfig, bool noWindow)
{
    Uint32 windowID = -1;
    if (!initted) {
        Locator::videoManager = new VideoManager;
        windowID = Locator::videoManager->initSystem(screenConfig, noWindow);

        Locator::logger = new Logger;
        Locator::soundPlayer = &(SoundPlayer::getSoundPlayer());
        Locator::soundPlayer->setState(
            SoundState::PLAY);  // should change this to be an enum
        Locator::keyboard = new Keyboard;

        const int NUMMOUSEBUTTONS = 3;
        Locator::mouse = new Mouse(NUMMOUSEBUTTONS);
        Locator::eventDispatcher = new EventDispatcher(Locator::videoManager);
        Locator::eventSubscriber =
            new EventSubscriber(Locator::eventDispatcher);
        Locator::fontManager = new FontManager();

        Locator::assetManager = nullptr;

        // initialize ControllerManager
        ControllerManager::initialize();

        // initialise 2d layer types
        LayerFactory& layerFactory = LayerFactory::getInstance();
        ImageLayer::registerConstructor(layerFactory);
        BitmapCollisionLayer::registerConstructor(layerFactory);

        // initialize components
        ComponentFactory& componentFactory = ComponentFactory::getInstance();
        PlaceHolderGraphics::registerConstructor(componentFactory);
        BoxCollider::registerConstructor(componentFactory);

        // initialise logging
        

        initted = true;
    }

    return windowID;
}

void destroy()
{
    if (initted) {
        delete Locator::mouse;
        delete Locator::keyboard;
        if (Locator::assetManager != nullptr) {
            delete Locator::assetManager;
        }
        delete Locator::eventDispatcher;
        delete Locator::soundPlayer;
        delete Locator::videoManager;
    }
}

void loadAssetFile(std::optional<std::string> assetsFile, std::optional<std::string> baseAssetPath)
{
    std::unique_ptr<AssetManager> pAssetManager(new AssetManager(
                                                    *(Locator::videoManager), *(Locator::soundPlayer), assetsFile, baseAssetPath));
    Locator::assetManager = pAssetManager.release();
}

void startLoop(std::shared_ptr<GameState> pGameState)
{
    Runner::getInstance().pushState(std::move(pGameState));
    Runner::getInstance().loop();
}

void end() { Runner::getInstance().end(); }

void switchState(std::shared_ptr<GameState> pGameState)
{
    Runner::getInstance().switchState(std::move(pGameState));
}

void popState() { Runner::getInstance().popState(); }

void pushState(std::shared_ptr<GameState> pGameState)
{
    Runner::getInstance().pushState(std::move(pGameState));
}

//! get the state at the top of the stack
/**
*/
std::shared_ptr<GameState> peekState()
{
    return Runner::getInstance().peekState();
}

//! creates a Window Widget
/**

   The WindowWidget gets inserted into the game loop and automatically
   gets update() and render() called on it.

   * \param name - The name of the window
   * \param width - The width of the window
   * \param height - The height of the window
   * \param resizable - Flag indicating if the window is resizable
   * \return The WindowWidget
   */
std::shared_ptr<UI::WindowWidget> createWindow(const std::string& name,
                                               int width, int height,
                                               bool resizable)
{
    std::shared_ptr<UI::WindowWidget> pWindowWidget =
        UI::WindowWidget::create(name, width, height, resizable);

    CAP_THROW_ASSERT(Locator::eventSubscriber != nullptr,
                     "Locator::eventSubscriber is null");
    pWindowWidget->registerSignals(*Locator::eventSubscriber);

    return pWindowWidget;
}

//! Enables/Disables default quit events (window close and q key)
/**
*/
void setDefaultQuitEvents(bool enabled)
{
    Runner::getInstance().setDefaultQuitEvents(enabled);
}

void setAssetPath(std::filesystem::path path)
{
    if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        BOOST_THROW_EXCEPTION(CapEngineException("Asset path does not exist or is not a directory"));
    }

    Locator::insertOrReplace(std::string{kAssetFileProperty}, path);
}

std::filesystem::path getAssetPath()
{
    auto maybePath = Locator::locate(std::string{kAssetFileProperty});
    if (maybePath.has_value()) {
        return std::any_cast<std::filesystem::path>(maybePath);
    }

    return std::filesystem::current_path();
}

}  // namespace CapEngine
