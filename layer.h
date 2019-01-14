#ifndef CAPENGINE_LAYER_H
#define CAPENGINE_LAYER_H

#include "collision.h"
#include "CapEngineException.h"

#include <optional>
#include <boost/variant.hpp>

namespace CapEngine {

// forward declarations
class Camera2d;
class GameObject;

//! Exception class for layer creation errors.
struct LayerCreationError : public CapEngineException {
	LayerCreationError(const std::string &in_type, const std::string &in_details)
		: CapEngineException([&]()
												 {
													 std::stringstream error;
													 error << "Error create layer.  type: " << in_type << " details: "
																 << std::endl << in_details;
													 return error.str();
												 }())
	{
	}
};
  
class Layer {
public:
	using CollidingShape = std::optional<boost::variant<Rectangle>>;
      
public:
	virtual ~Layer() = default;

	virtual void update(double in_ms) = 0;
	virtual void render(const Camera2d &in_camera, uint32_t in_windowId) = 0;
	virtual bool canCollide() { return false; }
	virtual CollidingShape checkCollision(const GameObject& in_object) { return std::nullopt; }
};

/** 
		\fn Layer::update
		\brief update funtion for the layer.
		\param in_ms
		The timestep.
*/

/** 
		\fn Layer::render
		\brief render funtion for the layer.
		\param in_camera
		The camera used for rendering.
		\param in_windowId
		The id of the window to render to.
*/


/** 
		\fn Layer::canCollid
		\brief Tells if layer is collidable with game objects.
		\return 
		true of it does collisions, false otherwise.
*/


/** 
		\fn Lyer::checkCollision
		\brief Check if a game object collides with anything in the layer.
		\param in_object
		The object to check collisions with.
		\return 
		The shape that collides with the object on the layer.
*/

} // CapEngine

#endif // CAPENGINE_LAYER_H
