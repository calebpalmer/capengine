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

    static VideoManager *videoManager;
    static Logger *logger;
    static SoundPlayer *soundPlayer;
    static Keyboard *keyboard;
    static Mouse *mouse;
    static AssetManager *assetManager;
    static EventDispatcher *eventDispatcher;
    static EventSubscriber *eventSubscriber;

    static VideoManager &getVideoManager();
    static Logger &getLogger();
    static SoundPlayer &getSoundPlayer();
    static Keyboard &getKeyboard();
    static Mouse &getMouse();
    static AssetManager &getAssetManager();
    static EventDispatcher &getEventDispatcher();
    static EventSubscriber &getEventSubscriber();
};

} // namespace CapEngine

#endif
