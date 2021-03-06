#include "game_management.h"

#include "bitmapcollisionlayer.h"
#include "boxcollider.h"
#include "componentfactory.h"
#include "controller.h"
#include "filesystem.h"
#include "imagelayer.h"
#include "layerfactory.h"
#include "locator.h"
#include "placeholdergraphics.h"
#include "runner.h"
#include "windowwidget.h"

#include <memory>
#include <sstream>

namespace CapEngine
{

bool initted = false;

Uint32 init(WindowParams screenConfig, bool noWindow)
{
  Uint32 windowID = -1;
  if (!initted) {
    std::unique_ptr<VideoManager> pVideoManager(new VideoManager());
    Locator::videoManager = pVideoManager.release();
    windowID = Locator::videoManager->initSystem(screenConfig, noWindow);

    std::unique_ptr<Logger> pLogger(new Logger);
    Locator::logger = pLogger.release();

    Locator::soundPlayer = &(SoundPlayer::getSoundPlayer());
    Locator::soundPlayer->setState(
        SoundState::PLAY); // should change this to be an enum

    std::unique_ptr<Keyboard> pKeyboard(new Keyboard);
    Locator::keyboard = pKeyboard.release();

    const int NUMMOUSEBUTTONS = 3;
    std::unique_ptr<Mouse> pMouse(new Mouse(NUMMOUSEBUTTONS));
    Locator::mouse = pMouse.release();

    std::unique_ptr<EventDispatcher> pEventDispatcher(
        new EventDispatcher(Locator::videoManager));
    Locator::eventDispatcher = pEventDispatcher.release();

    std::unique_ptr<EventSubscriber> pEventSubscriber(
        new EventSubscriber(*(Locator::eventDispatcher)));
    Locator::eventSubscriber = pEventSubscriber.release();

    Locator::assetManager = nullptr;

    // initialize ControllerManager
    ControllerManager::initialize();

    // set current directory to directory of executable
    // std::string exePath = getCurrentExecutablePath();
    // setCurrentDir(stripPath(exePath));

    // initialise 2d layer types
    LayerFactory &layerFactory = LayerFactory::getInstance();
    ImageLayer::registerConstructor(layerFactory);
    BitmapCollisionLayer::registerConstructor(layerFactory);

    // initialize components
    ComponentFactory &componentFactory = ComponentFactory::getInstance();
    PlaceHolderGraphics::registerConstructor(componentFactory);
    BoxCollider::registerConstructor(componentFactory);

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

void loadAssetFile(std::string assetsFile)
{
  std::unique_ptr<AssetManager> pAssetManager(new AssetManager(
      *(Locator::videoManager), *(Locator::soundPlayer), assetsFile));
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
    \return
    The GameState
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
std::shared_ptr<UI::WindowWidget>
    createWindow(const std::string &name, int width, int height, bool resizable)
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
    \param enabled
    if true then enabled, false then disabled.
*/
void setDefaultQuitEvents(bool enabled)
{
  Runner::getInstance().setDefaultQuitEvents(enabled);
}

} // namespace CapEngine
