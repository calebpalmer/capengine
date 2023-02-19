#include "layerfactory.h"

#include "layer.h"
#include "scene2dschema.h"

namespace CapEngine
{

//! Get the singleton instance
/**
 \return
   The singleton instance.
*/
LayerFactory &LayerFactory::getInstance()
{
  static LayerFactory layerFactory;
  return layerFactory;
}

//! The factory method.
/**
 \param in_json
   The json describing the layer to create.
 \return
   The layer.
*/
std::unique_ptr<Layer> LayerFactory::makeLayer(const jsoncons::json &in_json)
{
  if (!in_json.contains(Schema::Scene2d::kLayerType)) {
    throw LayerCreationException(
        "Unknown", in_json,
        std::string("The layer type could not be determined"));
  }

  const std::string type =
      in_json[Schema::Scene2d::kLayerType].as<std::string>();
  auto &&f = m_factoryFunctions.find(type);
  if (f != m_factoryFunctions.end()) {
    return f->second(in_json);
  }

  throw LayerCreationException(
      type, in_json,
      std::string("No factory method registered for layer type"));
}

//! Register a new type of layer that can be created.
/**
 \param in_type
   The type to create.
 \param in_factoryFunction
   The function.
*/
void LayerFactory::registerLayerType(const std::string &in_type,
                                     factoryfunc_t in_factoryFunction)
{
  if (m_factoryFunctions.find(in_type) == m_factoryFunctions.end()) {
    m_factoryFunctions.emplace(in_type, std::move(in_factoryFunction));
  }

  else {
    throw CapEngineException("The layer type \"" + in_type +
                             "\" is already registered");
  }
}

} // namespace CapEngine
