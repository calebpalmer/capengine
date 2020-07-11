#ifndef CAPENGINE_COMPONENTFACTORY_H
#define CAPENGINE_COMPONENTFACTORY_H

#include "CapEngineException.h"
#include "componentutils.h"

#include <functional>
#include <map>
#include <memory>

#include <jsoncons/json.hpp>

namespace CapEngine
{

//! Exception class for scene that does not exist.
struct ComponentCreationException : public CapEngineException {
  ComponentCreationException(const std::string &in_type,
                             const std::string &in_subtype,
                             const jsoncons::json &in_json,
                             const std::string &in_details)
      : CapEngineException([&]() {
          std::stringstream msg;
          msg << "Component of type \"" << in_type << "/" << in_subtype
              << "\" could not be created." << std::endl
              << "Details: " << in_details << std::endl
              << jsoncons::pretty_print(in_json);
          return msg.str();
        }())
  {
  }

  ComponentCreationException(ComponentType in_type,
                             const std::string &in_subtype,
                             const jsoncons::json &in_json,
                             const std::string &in_details)
      : ComponentCreationException(
            ComponentUtils::componentTypeToString(in_type), in_subtype, in_json,
            in_details)
  {
  }
};

class Component; // forward declaration

//! class for registering json component factory methods
class ComponentFactory final
{
public:
  using factoryfunc_t =
      std::function<std::unique_ptr<Component>(const jsoncons::json &)>;

public:
  ComponentFactory(const ComponentFactory &) = delete;

  static ComponentFactory &getInstance();

  std::unique_ptr<Component> makeComponent(const jsoncons::json &in_json);
  void registerComponentType(const std::string &in_type,
                             const std::string &in_subtype,
                             factoryfunc_t in_factoryFunction);

private:
  ComponentFactory() = default;

  //! map of the factory functions
  std::map<std::pair<std::string, std::string>, factoryfunc_t>
      m_factoryFunctions;
};

} // namespace CapEngine

#endif // CAPENGINE_COMPONENTFACTORY_H
