#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>

#include "vector.h"
#include "collision.h"
#include "components.h"

namespace CapEngine {

  class GameObject;

  class CollisionEvent{
  public:
    GameObject* object1;
    GameObject* object2;
    CollisionType type;
    CollisionClass class_;
  };

  class ObjectCreator{
  public:
    virtual std::unique_ptr<GameObject> createObject() = 0;
    virtual ~ObjectCreator() {}
  };

  class ObjectData {
  public:
    virtual ~ObjectData() {};
  };

  typedef long ObjectID;

  class GameObject{
  public:
    enum ObjectState {
      Inactive,
      Starting,
      Active,
      Dying,
      Dead
    };

  private:
    GameObject(const GameObject&);
    GameObject& operator=(const GameObject&);

    static ObjectID nextID;
    static int nextMessageId;
    std::shared_ptr<ObjectData> m_pObjectData;
    ObjectState m_objectState;
    ObjectID m_objectID;
    ObjectID m_parentObjectID;

    std::shared_ptr<InputComponent> inputComponent;
    std::shared_ptr<PhysicsComponent> physicsComponent;
    std::shared_ptr<GraphicsComponent> graphicsComponent;
    std::shared_ptr<CustomComponent> customComponent;
    std::shared_ptr<AIComponent> mpAIComponent;
    Vector position;
    Vector orientation;
    Vector velocity;
    Vector acceleration;


  public:

    //constructors
    GameObject(bool newID = true);
    ~GameObject();

    static ObjectID generateID();
    static int generateMessageId();
    void render();
    std::unique_ptr<GameObject> update(double ms) const;
    Rectangle boundingPolygon() const;
    bool handleCollision(CollisionType, CollisionClass, GameObject* otherObject,
			 Vector collisionLocation);
    std::unique_ptr<GameObject> clone() const;
    // getters and setters
    std::shared_ptr<ObjectData> getObjectData() const;
    void setObjectData(std::shared_ptr<ObjectData> pObjectData);
    ObjectState getObjectState() const;
    void setObjectState(ObjectState objectState);
    ObjectID getObjectID() const;
    void setObjectID(ObjectID id);
    ObjectID getParentObjectID() const;
    void send(int messageId, std::string message);
    void setParentObjectID(ObjectID id);
    void setInputComponent(std::shared_ptr<InputComponent> pInputComponent);
    void setPhysicsComponent(std::shared_ptr<PhysicsComponent> pPhysicsComponent);
    std::shared_ptr<PhysicsComponent> getPhysicsComponent() const;
    void setGraphicsComponent(std::shared_ptr<GraphicsComponent> pGraphicsComponent);
    void setCustomComponent(std::shared_ptr<CustomComponent> pCustomComponent);
    void setAIComponent(std::shared_ptr<AIComponent> pAIComponent);
    Vector getPosition() const;
    void setPosition(Vector position);
    Vector getOrientation() const;
    void setOrientation(Vector orientation);
    Vector getVelocity() const;
    void setVelocity(Vector velocity);
    Vector getAcceleration() const;
    void setAcceleration(Vector velocity);
  };
}

#endif // GAMEOBJECT_H
