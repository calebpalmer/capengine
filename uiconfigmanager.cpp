#include "uiconfigmanager.h"

#include "CapEngineException.h"
#include "filesystem.h"
#include "locator.h"
#include "logger.h"

#include <fstream>
#include <jsoncons_ext/jsonpath/json_query.hpp>
#include <memory>

namespace CapEngine
{
namespace UI
{

namespace
{

//! the singleton instance
static std::shared_ptr<UIConfigManager> spUIConfigManager;

//! The path to the config file
const std::string kConfigFileLocation = "res/uiconfig.json";
} // namespace

//! Constructor
UIConfigManager::UIConfigManager()
{
  // try to read in the config settings
  std::ifstream is(kConfigFileLocation);
  if (!is.is_open()) {
    const std::string message = "Unable to open file " + getCurrentDir() +
                                std::string("/") + kConfigFileLocation;
    CAP_THROW(CapEngineException(message));
  }

  is >> m_settings;
}

//! Gets a reference to the single instance
/**
 \return - The single instance
*/
UIConfigManager &UIConfigManager::getInstance()
{
  if (!spUIConfigManager)
    spUIConfigManager.reset(new UIConfigManager);

  return *spUIConfigManager;
}

//! returns a setting value.
/**
 \param settingPath - the JSONPath to the setting to retrieve.
 \return - The value of the setting.
*/
boost::optional<jsoncons::json>
    UIConfigManager::getSetting(const std::string &settingPath)
{
  jsoncons::json result =
      jsoncons::jsonpath::json_query(m_settings, settingPath);

  try {
    if (result.size() > 0)
      return result;

    else
      return boost::none;
  } catch (const std::exception &e) {
    assert(Locator::logger != nullptr);
    Locator::logger->log(boost::diagnostic_information(e), Logger::CWARNING);
    return boost::none;
  }
}

//! Extracts a colour from json
/**
 \param json - The json to extractfrom
 \return - A Colour if it could, boost::none if it could not.
*/
boost::optional<Colour> getColour(const jsoncons::json &object)
{
  if (!object.has_key("r") || !object.has_key("g") || !object.has_key("b"))
    return boost::none;

  Colour colour = {object["r"].as<uint8_t>(), object["g"].as<uint8_t>(),
                   object["b"].as<uint8_t>(), 255};

  if (object.has_key("a"))
    colour.m_a = object["a"].as<uint8_t>();

  return colour;
}

//! Gets a property from a json or return a default value if it doesn't exist.
/**
 \param object - The object to get the property from.
 \param propertyName - The name of the property to get.
 \param defaultValue - The default value to returu if the property isn't found
 \return - The property.
*/
jsoncons::json getProperty(jsoncons::json object,
                           const std::string &propertyName,
                           jsoncons::json defaultValue)
{
  if (!object.has_key(propertyName))
    return defaultValue;
  else
    return object[propertyName];
}

} // namespace UI
} // namespace CapEngine
