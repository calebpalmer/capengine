#ifndef CAPENGINE_LAYER_H
#define CAPENGINE_LAYER_H

#include "CapEngineException.h"
#include "collision.h"
#include "gameobject.h"

#include <boost/variant.hpp>
#include <optional>

namespace CapEngine
{

// forward declarations
class Camera2d;
class GameObject;

//! Exception class for layer creation errors.
struct LayerCreationError : public CapEngineException {
	LayerCreationError(const std::string &in_type,
					   const std::string &in_details)
		: CapEngineException([&]() {
			  std::stringstream error;
			  error << "Error create layer.  type: " << in_type
					<< " details: " << std::endl
					<< in_details;
			  return error.str();
		  }())
	{
	}
};

class Layer
{
  public:
	using CollisionType_t =
		std::vector<std::pair<CapEngine::CollisionType, Vector>>;

  public:
	virtual ~Layer() = default;

	virtual void update(double in_ms) = 0;
	virtual void render(const Camera2d &in_camera, uint32_t in_windowId) = 0;
	virtual const std::string type() const = 0;
	virtual bool canCollide() const { return false; }
	virtual CollisionType_t
		checkCollisions(const GameObject & /*in_object*/) const
	{
		return {};
	}
	virtual bool resolveCollisions(GameObject & /*in_object*/) const
	{
		return false;
	}
};

/**
																																\fn
   Layer::update \brief update funtion for the layer. \param in_ms The timestep.
*/

/**
																																\fn
   Layer::render \brief render funtion for the layer. \param in_camera The
   camera used for rendering. \param in_windowId The id of the window to render
   to.
*/

/**
																																\fn
   Layer::canCollid \brief Tells if layer is collidable with game objects.
   \return true of it does collisions, false otherwise.
*/

/**
																																\fn
   Lyer::checkCollision \brief Check if a game object collides with anything in
   the layer. \param in_object The object to check collisions with. \return The
   shape that collides with the object on the layer.
*/

} // namespace CapEngine



#endif // CAPENGINE_LAYER_H
