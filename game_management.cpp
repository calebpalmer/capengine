#include "game_management.h"
#include "locator.h"
#include "filesystem.h"
#include "runner.h"

#include <memory>
#include <sstream>

using namespace CapEngine;

bool CapEngine::initted = false;

void CapEngine::init(Screen_t screenConfig){
  if(!initted){
    std::unique_ptr<VideoManager> pVideoManager(new VideoManager());
    Locator::videoManager = pVideoManager.release();
    Locator::videoManager->initSystem(screenConfig);

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

    Locator::assetManager = nullptr;

    // set current directory to directory of executable
    std::string exePath = getCurrentExecutablePath();
    setCurrentDir(stripPath(exePath));
    initted = true;
  }
}

void CapEngine::destroy(){
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

void CapEngine::loadAssetFile(std::string assetsFile){
  std::unique_ptr<AssetManager> pAssetManager(new AssetManager(*(Locator::videoManager), *(Locator::soundPlayer), assetsFile));
  Locator::assetManager = pAssetManager.release();
}

void CapEngine::startLoop(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().pushState(std::move(pGameState));
  Runner::getInstance().loop();
}

void CapEngine::end(){
  Runner::getInstance().end();
}

void CapEngine::switchState(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().switchState(std::move(pGameState));
}

void CapEngine::popState(){
  Runner::getInstance().popState();
}

void CapEngine::pushState(std::unique_ptr<GameState> pGameState){
  Runner::getInstance().pushState(std::move(pGameState));
}

