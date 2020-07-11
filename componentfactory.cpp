
#include "componentfactory.h"

#include "components.h"
#include "scene2dschema.h"

namespace CapEngine
{

//! Get the singleton instance
/**
 \return
   The singleton instance.
*/
ComponentFactory &ComponentFactory::getInstance()
{
  static ComponentFactory layerFactory;
  return layerFactory;
}

//! The factory method.
/**
 \param in_json
   The json describing the component to create.
 \return
   The component.
*/
std::unique_ptr<Component>
    ComponentFactory::makeComponent(const jsoncons::json &in_json)
{
  if (!in_json.contains(Schema::Scene2d::kType)) {
    throw ComponentCreationException(
        "Unknown", "Unknown", in_json,
        std::string("The component type could not be determined"));
  }

  const std::string type =
      in_json[Schema::Components::kComponentType].as<std::string>();
  const std::string subtype =
      in_json[Schema::Components::kComponentSubType].as<std::string>();

  auto &&f = m_factoryFunctions.find(std::make_pair(type, subtype));
  if (f != m_factoryFunctions.end()) {
    return f->second(in_json);
  }

  throw ComponentCreationException(
      type, subtype, in_json,
      std::string("No factory method registered for layer type"));
}

//! Register a new type of component that can be created.
/**
 \param in_type
   The type to create.
 \param in_factoryFunction
   The function.
*/
void ComponentFactory::registerComponentType(const std::string &in_type,
                                             const std::string &in_subtype,
                                             factoryfunc_t in_factoryFunction)
{
  auto key = std::make_pair(in_type, in_subtype);
  if (m_factoryFunctions.find(key) == m_factoryFunctions.end()) {
    m_factoryFunctions.emplace(key, std::move(in_factoryFunction));
  } else {
    throw CapEngineException("The component type \"" + in_type +
                             "\" is already registered");
  }
}

} // namespace CapEngine
