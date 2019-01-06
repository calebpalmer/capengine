#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <memory>
#include <string>

#include "vector.h"
#include "collision.h"
#include "components.h"

namespace CapEngine {

  // forward declaractions
  class Camera2d;
  class GameObject;

  class CollisionEvent{
  public:
    GameObject* object1;
    GameObject* object2;
    CollisionType type;
    CollisionClass class_;

    friend std::ostream& operator<<(std::ostream& stream, const CollisionEvent collisionEvent);
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

    enum ObjectType {
      ObjectType_AI,
      ObjectType_Player
    };

    //constructors
    GameObject(bool newID = true);
    ~GameObject();

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    static ObjectID generateID();
    static int generateMessageId();
    void render(const Camera2d &in_camera);
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
		void setComponent(std::shared_ptr<Component> pComponent);
    void setInputComponent(std::shared_ptr<InputComponent> pInputComponent);
    std::shared_ptr<InputComponent> getInputComponent();
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
    Vector getPreviousPosition() const;
    void setPreviousPosition(Vector position);
    ObjectType getObjectType() const;
    void setObjectType(ObjectType in_objectType);

    friend std::ostream& operator<<(std::ostream& stream, GameObject const& object);


   private:
    static ObjectID nextID;
    static int nextMessageId;
    std::shared_ptr<ObjectData> m_pObjectData;
    ObjectState m_objectState = Active;
    ObjectID m_objectID = -1;
    ObjectID m_parentObjectID = -1;
    ObjectType m_objectType = ObjectType_AI;

    std::shared_ptr<InputComponent> inputComponent;
    std::shared_ptr<PhysicsComponent> physicsComponent;
    std::shared_ptr<GraphicsComponent> graphicsComponent;
    std::shared_ptr<CustomComponent> customComponent;
    std::shared_ptr<AIComponent> mpAIComponent;
    Vector position;
    Vector previousPosition;
    Vector orientation;
    Vector velocity;
    Vector acceleration;
  };
}

#endif // GAMEOBJECT_H
