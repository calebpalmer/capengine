#ifndef CAPENGINE_COMPONENTUTILS_H
#define CAPENGINE_COMPONENTUTILS_H

#include "components.h"
#include <string_view>

#include <jsoncons/json.hpp>

namespace CapEngine
{
namespace ComponentUtils
{

std::string componentTypeToString(ComponentType in_componentType);
void assertPropertyExists(const jsoncons::json &in_json,
                          const std::string &propertyName);

} // namespace ComponentUtils
} // namespace CapEngine

#endif // CAPENGINE_COMPONENTUTILS_H
