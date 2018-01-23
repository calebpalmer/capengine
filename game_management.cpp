#include "game_management.h"

#include "locator.h"
#include "filesystem.h"
#include "runner.h"
#include "controller.h"
#include "windowwidget.h"

#include <memory>
#include <sstream>

namespace CapEngine {

bool initted = false;

Uint32 init(WindowParams screenConfig, bool noWindow){
  Uint32 windowID = -1;
  if(!initted){
    std::unique_ptr<VideoManager> pVideoManager(new VideoManager());
    Locator::videoManager = pVideoManager.release();
    windowID = Locator::videoManager->initSystem(screenConfig, noWindow);

    std::unique_ptr<Logger> pLogger(new Logger);
    //Locator::logger = pLogger.release();
    Locator::logger = new Logger;

    Locator::soundPlayer = &(SoundPlayer::getSoundPlayer());
    Locator::soundPlayer->setState(SoundState::PLAY);  // should change this to be an enum

    std::unique_ptr<Keyboard> pKeyboard(new Keyboard);
    Locator::keyboard = pKeyboard.release();

    const int NUMMOUSEBUTTONS = 3;
    std::unique_ptr<Mouse> pMouse(new Mouse(NUMMOUSEBUTTONS));
    Locator::mouse = pMouse.release();

    std::unique_ptr<EventDispatcher> pEventDispatcher(new EventDispatcher(Locator::videoManager));
    Locator::eventDispatcher = pEventDispatcher.release();

		std::unique_ptr<EventSubscriber> pEventSubscriber(new EventSubscriber(*(Locator::eventDispatcher)));
		Locator::eventSubscriber = pEventSubscriber.release();

    Locator::assetManager = nullptr;

    // initialize ControllerManager
    ControllerManager::initialize();

    // set current directory to directory of executable
    std::string exePath = getCurrentExecutablePath();
    setCurrentDir(stripPath(exePath));
    initted = true;
  }

  return windowID;
}

void destroy(){
  if(initted){
    delete Locator::mouse;
    delete Locator::keyboard;
    if(Locator::assetManager != nullptr){
      delete Locator::assetManager;
    }
    delete Locator::eventDispatcher;
    delete Locator::soundPlayer;
    delete Locator::videoManager;
  }
}

void loadAssetFile(std::string assetsFile){
  std::unique_ptr<AssetManager> pAssetManager(new AssetManager(*(Locator::videoManager), *(Locator::soundPlayer), assetsFile));
  Locator::assetManager = pAssetManager.release();
}

void startLoop(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().pushState(std::move(pGameState));
  Runner::getInstance().loop();
}

void end(){
  Runner::getInstance().end();
}

void switchState(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().switchState(std::move(pGameState));
}

void popState(){
  Runner::getInstance().popState();
}

void pushState(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().pushState(std::move(pGameState));
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
std::shared_ptr<UI::WindowWidget> createWindow(const std::string &name, int width, int height, bool resizable){
	std::shared_ptr<UI::WindowWidget> pWindowWidget =
		std::make_shared<UI::WindowWidget>(name , width, height, resizable);
	
	CAP_THROW_ASSERT(Locator::eventSubscriber != nullptr, "Locator::eventSubscriber is null");
	pWindowWidget->registerSignals(*Locator::eventSubscriber);

	return pWindowWidget;
}

}
