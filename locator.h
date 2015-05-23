#ifndef LOCATOR_H
#define LOCATOR_H

#include "VideoManager.h"
#include "logger.h"
#include "soundplayer.h"
#include "keyboard.h"
#include "mouse.h"
#include "asset_manager.h"
#include "EventDispatcher.h"

class Locator{

 public:
  static CapEngine::VideoManager* videoManager;
  static CapEngine::Logger* logger;
  static CapEngine::SoundPlayer* soundPlayer;
  static CapEngine::Keyboard* keyboard;
  static CapEngine::Mouse* mouse;
  static CapEngine::AssetManager* assetManager;
  static CapEngine::EventDispatcher* eventDispatcher;
};

#endif
