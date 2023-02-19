#ifndef CAPENGINE_UICONFIGMANAGER_H
#define CAPENGINE_UICONFIGMANAGER_H

#include "colour.h"

#include <boost/optional.hpp>
#include <jsoncons/json.hpp>

namespace CapEngine
{
namespace UI
{

// settings paths
const std::string kDefaultFontSettingsPath = "$.font.path";
const std::string kDefaultFontSizeSettingsPath = "$.font.size";

// defaults
const int kDefaultFontSize = 13;

class UIConfigManager
{
public:
  static UIConfigManager &getInstance();

  boost::optional<jsoncons::json> getSetting(const std::string &settingPath);

private:
  UIConfigManager();

  jsoncons::json m_settings;
};

boost::optional<Colour> getColour(const jsoncons::json &object);
jsoncons::json
    getProperty(jsoncons::json object, const std::string &propertyName,
                jsoncons::json defaultValue = jsoncons::json::object());

} // namespace UI
} // namespace CapEngine

#endif // CAPENGINE_UICONFIGMANAGERH_
