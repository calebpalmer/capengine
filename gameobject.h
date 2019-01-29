#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "vector.h"
#include "collision.h"
#include "components.h"

#include <memory>
#include <string>
#include <optional>

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
		
    ~GameObject() = default;
    GameObject(const GameObject&) = default;
    GameObject& operator=(const GameObject&) = default;

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

		void addComponent(std::shared_ptr<Component> in_pComponent);
		const std::vector<std::shared_ptr<Component>>& getComponents();
		std::vector<std::shared_ptr<Component>> getComponents(ComponentType in_type);
		
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

		//! The components
		std::vector<std::shared_ptr<Component>> m_components;

    Vector position;
    Vector previousPosition;
    Vector orientation;
    Vector velocity;
    Vector acceleration;
  };
}

#endif // GAMEOBJECT_H
 
