#ifndef CAPENGINE_SCENE2D_H
#define CAPENGINE_SCENE2D_H

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
};

}

// include implementation
#include "scene2d.tpp"

#endif // CAPENGINE_SCENE2D

