#ifndef CAPENGINE_SCENE2D_H
#define CAPENGINE_SCENE2D_H

#include "collision.h"
#include "camera2d.h"

#include <jsoncons/json.hpp>

namespace CapEngine {

//! class for representing a game scene.
template <class ObjectManager>
class Scene2d final {
public:
	explicit Scene2d(const jsoncons::json &in_json);
	explicit Scene2d(const jsoncons::json &in_json, ObjectManager in_objectManager);

	void update(double in_ms);
	void render(uint32_t in_windowId);
	
private:
	ObjectManager m_objectManager; //<! Holds the objects and performs collision checking.
	std::string m_sceneID; //<! The id of the scene.
	Rectangle m_sceneSize;  //<! The size of the scene.
	//Camera2d m_camera;  //<! The camera.
};

}

// include implementation
#include "scene2d.tpp"

#endif // CAPENGINE_SCENE2D

