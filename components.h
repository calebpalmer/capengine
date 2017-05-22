#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <memory>

namespace CapEngine {
  class GameObject;

  class Component{
  public:
    virtual ~Component() {}
    virtual void receive(GameObject* object, int messageId, std::string message) {}
  };
  
  class PhysicsComponent : public Component {
  public:
    virtual ~PhysicsComponent() {}
    virtual void update(GameObject* object, double timestep) = 0;
    virtual Rectangle boundingPolygon(const GameObject* object) const = 0;
    virtual bool handleCollision(GameObject* object, CollisionType, CollisionClass, GameObject* otherObject,
				 Vector collisionVector) = 0;
    virtual bool handlesCollisions() {return false;}
  };

  class GraphicsComponent : public Component {
  public:
    virtual ~GraphicsComponent() {}
    virtual void render(GameObject* object) = 0;
    virtual void update(GameObject* pObject, double timestepMS) = 0;
  };

  class InputComponent : public Component {
  public:
    virtual ~InputComponent() {}
    virtual void update(GameObject* object) = 0;
  };

  class CustomComponent : public Component {
  public:
    virtual ~CustomComponent() {}
    virtual void update(GameObject* object) = 0;
    virtual std::unique_ptr<CustomComponent> clone() const = 0;
  };

  class AIComponent : public Component {
  public:
    virtual ~AIComponent() {}
    virtual void update(GameObject* object) = 0;
    virtual std::unique_ptr<AIComponent> clone() const = 0;
  };

}

#endif // COMPONENTS_H
