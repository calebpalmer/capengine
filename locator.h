#ifndef LOCATOR_H
#define LOCATOR_H

#include "EventDispatcher.h"
#include "VideoManager.h"
#include "asset_manager.h"
#include "eventsubscriber.h"
#include "keyboard.h"
#include "logger.h"
#include "mouse.h"
#include "soundplayer.h"

#include <any>
#include <boost/any.hpp>
#include <optional>

namespace CapEngine
{

class Locator
{

  public:
    static bool insert(const std::string &id, std::any item);
    static void insertOrReplace(const std::string &id, std::any item);
    static std::any locate(const std::string &id);

    static CapEngine::VideoManager *videoManager;
    static CapEngine::Logger *logger;
    static CapEngine::SoundPlayer *soundPlayer;
    static CapEngine::Keyboard *keyboard;
    static CapEngine::Mouse *mouse;
    static CapEngine::AssetManager *assetManager;
    static CapEngine::EventDispatcher *eventDispatcher;
    static CapEngine::EventSubscriber *eventSubscriber;
};

} // namespace CapEngine

#endif
