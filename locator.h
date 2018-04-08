#ifndef LOCATOR_H
#define LOCATOR_H

#include "VideoManager.h"
#include "logger.h"
#include "soundplayer.h"
#include "keyboard.h"
#include "mouse.h"
#include "asset_manager.h"
#include "EventDispatcher.h"
#include "eventsubscriber.h"

#include <boost/any.hpp>

namespace CapEngine{

  class Locator{

  public:

		static bool insert(const std::string &id, boost::any item);
		static void insertOrReplace(const std::string &id, boost::any item);		
		static boost::any locate(const std::string &id);
		
    static CapEngine::VideoManager* videoManager;
    static CapEngine::Logger* logger;
    static CapEngine::SoundPlayer* soundPlayer;
    static CapEngine::Keyboard* keyboard;
    static CapEngine::Mouse* mouse;
    static CapEngine::AssetManager* assetManager;
    static CapEngine::EventDispatcher* eventDispatcher;
		static CapEngine::EventSubscriber* eventSubscriber;
  };

}

#endif
