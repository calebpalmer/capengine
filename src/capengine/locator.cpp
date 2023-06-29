#include "locator.h"

#include <boost/throw_exception.hpp>

#include "CapEngineException.h"
#include "EventDispatcher.h"
#include "boost/optional.hpp"
#include "eventsubscriber.h"
#include "filesystem.h"

namespace CapEngine
{

VideoManager *Locator::videoManager = nullptr;
Logger *Locator::logger = nullptr;
SoundPlayer *Locator::soundPlayer = nullptr;
Keyboard *Locator::keyboard = nullptr;
Mouse *Locator::mouse = nullptr;
AssetManager *Locator::assetManager = nullptr;
EventDispatcher *Locator::eventDispatcher = nullptr;
EventSubscriber *Locator::eventSubscriber = nullptr;

namespace
{

//! The map containing the registered items
static std::map<std::string, std::any> itemMap;

}  // namespace

//! Insert an item with the Locator
/**

Throws if the
 item already exists.

 \param id - The id of the  item to register.
 \param item - The item to register.
 \returns - true if successful, false otherwise.
*/
bool Locator::insert(const std::string &id, std::any item)
{
    if (itemMap.find(id) == itemMap.end()) {
        itemMap.emplace(id, item);
        return true;
    } else
        return false;
}

//! Insert an item with the Locator.
/**

 \param id - The id of the  item to register.
 \param item - The item to register.
*/
void Locator::insertOrReplace(const std::string &id, std::any item)
{
    if (itemMap.find(id) == itemMap.end())
        itemMap.emplace(id, item);

    else
        itemMap[id] = item;
}

//! Locates a registered item.
/**
 \param id - The id of the item to locate.
 \return - The item if it's found, boost::none if not.
*/
std::any Locator::locate(const std::string &id)
{
    auto itemItr = itemMap.find(id);
    if (itemItr != itemMap.end())
        return itemItr->second;

    else
        return std::any{};
}

VideoManager &Locator::getVideoManager()
{
    if (videoManager == nullptr) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("video manager not initialized."));
    }

    return *videoManager;
}

Logger &Locator::getLogger()
{
    if (logger == nullptr) {
        BOOST_THROW_EXCEPTION(CapEngineException("logger not initialized."));
    }

    return *logger;
}

SoundPlayer &Locator::getSoundPlayer()
{
    if (soundPlayer == nullptr) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("soundPlayer not initialized."));
    }

    return *soundPlayer;
}

Keyboard &Locator::getKeyboard()
{
    if (keyboard == nullptr) {
        BOOST_THROW_EXCEPTION(CapEngineException("keyboard not initialized."));
    }

    return *keyboard;
}

Mouse &Locator::getMouse()
{
    if (mouse == nullptr) {
        BOOST_THROW_EXCEPTION(CapEngineException("mouse not initialized."));
    }

    return *mouse;
}

AssetManager &Locator::getAssetManager()
{
    if (assetManager == nullptr) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("assetManager not initialized."));
    }

    return *assetManager;
}

EventDispatcher &Locator::getEventDispatcher()
{
    if (eventDispatcher == nullptr) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("eventDispatcher not initialized."));
    }

    return *eventDispatcher;
}

EventSubscriber &Locator::getEventSubscriber()
{
    if (eventSubscriber == nullptr) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("eventSubscriber not initialized."));
    }

    return *eventSubscriber;
}

}  // namespace CapEngine
