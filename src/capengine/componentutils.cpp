#include "componentutils.h"
#include "CapEngineException.h"

namespace CapEngine
{
namespace ComponentUtils
{

//! Gets the string representation of a ComponentType
/**
 \param in_componentType
   The component type.
 \return
   The string.
*/
std::string componentTypeToString(ComponentType in_componentType)
{
  switch (in_componentType) {
  case ComponentType::Physics:
    return "Physics";
  case ComponentType::Graphics:
    return "Graphics";
  case ComponentType::Input:
    return "Input";
  case ComponentType::Custom:
    return "Custom";
  case ComponentType::AI:
    return "AI";
  default:
    return "Unknown";
  }
}

/**
 * @brief Throw an exception if property is missing.
 * @param[in] in_json The json to check.
 * @param[in] in_propertyName The property name to check.
 */
void assertPropertyExists(const jsoncons::json &in_json,
                          const std::string &in_propertyName)
{
  if (!in_json.contains(in_propertyName)) {
    std::ostringstream msg;
    msg << in_propertyName << " property is required.";
    CAP_THROW(CapEngineException(msg.str()));
  }
}

} // namespace ComponentUtils
} // namespace CapEngine
