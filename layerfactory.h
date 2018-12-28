#ifndef CAPENGINE_LAYERFACTORY_H
#define CAPENGINE_LAYERFACTORY_H

#include "CapEngineException.h"

#include <memory>
#include <functional>
#include <map>

#include <jsoncons/json.hpp>

namespace CapEngine {

//! Exception class for scene that does not exist.
struct LayerCreationException : public CapEngineException {
	explicit LayerCreationException(const std::string &in_type, const jsoncons::json &in_json, const std::string &in_details)
		: CapEngineException([&]()
												 {
													 std::stringstream msg;
													 msg << "Layer of type \"" << in_type
															 << "\" could not be created." << std::endl
															 << "Details: " << in_details << std::endl
															 << jsoncons::pretty_print(in_json);
													 return msg.str();
												 }())
	{
	}
};

class Layer; // forward declaration

class LayerFactory final {
public:
	using factoryfunc_t = std::function<std::unique_ptr<Layer>(const jsoncons::json&)>;
public:
	static LayerFactory& getInstance();

	std::unique_ptr<Layer> makeLayer(const jsoncons::json &in_json);
	void registerLayerType(const std::string &in_type, factoryfunc_t in_factoryFunction);

private:
	LayerFactory() = default;

	std::map<std::string, factoryfunc_t> m_factoryFunctions;
};

} // CapEngine

#endif // CAPENGINE_LAYERFACTORY_H
