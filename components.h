#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "camera2d.h"

#include <memory>

namespace CapEngine {
class GameObject;

//! The component type.
enum class ComponentType {
  Physics,
	Graphics,
	Input,
	Custom,
	AI
};

class Component{
public:
	virtual ~Component() = default;
	
	virtual void update(GameObject* object, double timestep) = 0;
	virtual void receive(GameObject* object, int messageId, std::string message) {}
	virtual ComponentType getType() const = 0;

	virtual std::unique_ptr<Component> clone() const = 0;
};
  
class PhysicsComponent : public Component {
public:
	virtual ~PhysicsComponent() {}
	virtual Rectangle boundingPolygon(const GameObject* object) const = 0;
	virtual bool handleCollision(GameObject* object, CollisionType, CollisionClass, GameObject* otherObject,
															 Vector collisionVector) = 0;
	virtual bool handlesCollisions() {return false;}
	ComponentType getType() const override { return ComponentType::Physics; }
};

class GraphicsComponent : public Component {
public:
	virtual ~GraphicsComponent() {}
	virtual void render(GameObject* object, const Camera2d &in_camera) = 0;
	ComponentType getType() const override { return ComponentType::Graphics; }
};

class InputComponent : public Component {
public:
	virtual ~InputComponent() {}
	ComponentType getType() const override { return ComponentType::Input; }
};

class CustomComponent : public Component {
public:
	virtual ~CustomComponent() {}
	ComponentType getType() const override { return ComponentType::Custom; }
};

class AIComponent : public Component {
public:
	virtual ~AIComponent() {}
	ComponentType getType() const override { return ComponentType::AI; }
};

}

#endif // COMPONENTS_H
