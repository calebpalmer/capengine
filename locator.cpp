#include "locator.h"
#include "filesystem.h"

#include "boost/optional.hpp"

namespace CapEngine {

namespace {

//! The map containing the registered items
std::map<std::string, boost::any> itemMap;

}


//! Insert an item with the Locator
/** 

		Throws if the item already exists.

 \param id - The id of the  item to register.
 \param item - The item to register.
 \returns - true if successful, false otherwise.
*/
bool Locator::insert(const std::string &id, boost::any item){
	if(itemMap.find(id) == itemMap.end()){
		itemMap.emplace(id, item);
		return true;
	}
	else
		return false;
}

//! Insert an item with the Locator.
/** 

		Throws if the item already exists.

 \param id - The id of the  item to register.
 \param item - The item to register.
*/
void Locator::insertOrReplace(const std::string& id, boost::any item){
	if(itemMap.find(id) == itemMap.end())
		itemMap.emplace(id, item);

	else
		itemMap[id] = item;
}


//! Locates a registered item.
/** 
 \param id - The id of the item to locate.
 \return - The item if it's found, boost::none if not.
*/
boost::any Locator::locate(const std::string &id){
	auto itemItr = itemMap.find(id);
	if(itemItr != itemMap.end())
		return *itemItr;

	else
		return boost::none;
}

VideoManager* Locator::videoManager = nullptr;
Logger* Locator::logger = nullptr;
SoundPlayer* Locator::soundPlayer = nullptr;
Keyboard* Locator::keyboard = nullptr;
Mouse* Locator::mouse = nullptr;
AssetManager* Locator::assetManager = nullptr;
EventDispatcher* Locator::eventDispatcher = nullptr;
EventSubscriber* Locator::eventSubscriber = nullptr;

}
