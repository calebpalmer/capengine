#ifndef CAPENGINE_SCENE2D_H
#define CAPENGINE_SCENE2D_H

#include "collision.h"
#include "camera2d.h"
#include "layer.h"
#include "CapEngineException.h"

#include <memory>
#include <map>
#include <jsoncons/json.hpp>

namespace CapEngine {

//! Exception class for scene that does not exist.
struct SceneLoadException : public CapEngineException {
	explicit SceneLoadException(const jsoncons::json in_json, const std::string &in_details)
		: CapEngineException([&]()
												 {
													 std::stringstream msg;
													 msg << "The scene could not be loaded.  Details:" << std::endl
															 << in_details << std::endl << jsoncons::pretty_print(in_json);
													 return msg.str();
												 }())
	{
	}
};

//! class for representing a game scene.
template <class ObjectManager>
class Scene2d final {
public:
	explicit Scene2d(const jsoncons::json &in_json);
	explicit Scene2d(const jsoncons::json &in_json, ObjectManager in_objectManager);

	void update(double in_ms);
	void render(uint32_t in_windowId);
		
private:
	void load(const jsoncons::json &in_json);
	
	ObjectManager m_objectManager; //<! Holds the objects and performs collision checking.
	std::multimap<int, std::unique_ptr<Layer>> m_layers; //<! Map of layers ordered by their drawing order.  0 = front.
	std::string m_sceneID; //<! The id of the scene.
	Rectangle m_sceneSize;  //<! The size of the scene.
	Camera2d m_camera;  //<! The camera.
};

}

// include implementation
#include "scene2d.tpp"

#endif // CAPENGINE_SCENE2D

