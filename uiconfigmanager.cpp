#include "uiconfigmanager.h"

#include <memory>

namespace CapEngine { namespace UI {

namespace {

static std::shared_ptr<UIConfigManager> spUIConfigManager;

}

//! Constructor
UIConfigManager::UIConfigManager(){}


//! Gets a reference to the single instance
/** 
 \return - The single instance
*/
UIConfigManager& UIConfigManager::getInstance(){
	if(!spUIConfigManager)
		spUIConfigManager.reset(new UIConfigManager);

	return *spUIConfigManager;
}


//! returns a setting value.
/** 
 \param settingPath - the path to the setting to retrieve.
 \return - The value of the setting.
*/
boost::optional<std::string> UIConfigManager::getSetting(const std::string &settingPath){
	if(settingPath == kDefaultFontSettingsPath)
		return std::string("res/fonts/tahoma.ttf");

	return boost::none;
}

}}
