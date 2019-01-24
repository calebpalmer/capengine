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
	virtual ~Component() {}
	virtual void receive(GameObject* object, int messageId, std::string message) {}
	virtual ComponentType getType() const = 0;
};
  
class PhysicsComponent : public Component {
public:
	virtual ~PhysicsComponent() {}
	virtual void update(GameObject* object, double timestep) = 0;
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
	virtual void update(GameObject* pObject, double timestepMS) = 0;
	ComponentType getType() const override { return ComponentType::Graphics; }
};

class InputComponent : public Component {
public:
	virtual ~InputComponent() {}
	virtual void update(GameObject* object) = 0;
	ComponentType getType() const override { return ComponentType::Input; }
};

class CustomComponent : public Component {
public:
	virtual ~CustomComponent() {}
	virtual void update(GameObject* object) = 0;
	virtual std::unique_ptr<CustomComponent> clone() const = 0;
	ComponentType getType() const override { return ComponentType::Custom; }
};

class AIComponent : public Component {
public:
	virtual ~AIComponent() {}
	virtual void update(GameObject* object) = 0;
	virtual std::unique_ptr<AIComponent> clone() const = 0;
	ComponentType getType() const override { return ComponentType::AI; }
};

}

#endif // COMPONENTS_H
