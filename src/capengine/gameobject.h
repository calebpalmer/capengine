#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <cstdint>
#include <memory>
#include <string>

#include "captypes.h"
#include "collision.h"
#include "components.h"
#include "gameevent.h"
#include "vector.h"

namespace CapEngine {

// forward declaractions
class Camera2d;
class GameObject;

class CollisionEvent {
   public:
    std::shared_ptr<GameObject> object1;
    std::shared_ptr<GameObject> object2;
    CollisionType type;
    CollisionClass class_;

    friend std::ostream& operator<<(std::ostream& stream, CollisionEvent const& collisionEvent);
};

class ObjectCreator {
   public:
    virtual std::unique_ptr<GameObject> createObject() = 0;
    virtual ~ObjectCreator() = default;
};

class ObjectData {
   public:
    virtual ~ObjectData() = default;
};

using ObjectID = int64_t;

//! Whether coordinate system as top is 0 or bottom is 0
enum class YAxisOrientation { TopZero, BottomZero };

class GameObject {
   public:
    enum ObjectState { Inactive, Starting, Active, Dying, Dead };

    enum ObjectType { ObjectType_AI, ObjectType_Player };

    // constructors
    explicit GameObject(bool newID = true);

    ~GameObject() = default;
    GameObject(const GameObject&) = default;
    GameObject& operator=(const GameObject&) = default;
    GameObject(GameObject&& in_other) noexcept = default;
    GameObject& operator=(GameObject&& in_other) noexcept = default;
    // defaulted copy cosntructor/operator might need to be revisited at some point
    // because it copies the shared_ptr to components rather than deep copies them.
    // if the component has state that doesn't work well with rollback then this can
    // cause issues

    void swap(GameObject& io_other) noexcept;

    static ObjectID generateID();
    static int generateMessageId();
    void render(const Camera2d& in_camera, uint32_t in_windowId);
    [[nodiscard]] std::unique_ptr<GameObject> update(double ms) const;
    [[nodiscard]] Rectangle boundingPolygon() const;
    bool handleCollision(CollisionType, CollisionClass, GameObject* otherObject, Vector const& collisionLocation);
    [[nodiscard]] std::unique_ptr<GameObject> clone() const;
    // getters and setters
    [[nodiscard]] std::shared_ptr<ObjectData> getObjectData() const;
    void setObjectData(std::shared_ptr<ObjectData> pObjectData);
    [[nodiscard]] ObjectState getObjectState() const;
    void setObjectState(ObjectState objectState);
    [[nodiscard]] ObjectID getObjectID() const;
    void setObjectID(ObjectID id);
    [[nodiscard]] ObjectID getParentObjectID() const;
    void send(int messageId, const std::string& message);
    void setParentObjectID(ObjectID id);

    void addComponent(std::shared_ptr<Component> in_pComponent);
    const std::vector<std::shared_ptr<Component>>& getComponents();
    std::vector<std::shared_ptr<Component>> getComponents(ComponentType in_type);
    template <typename T>
    std::vector<std::shared_ptr<T>> getComponents();

    [[nodiscard]] Vector const& getPosition() const;
    void setPosition(Vector position);
    [[nodiscard]] Vector const& getOrientation() const;
    void setOrientation(Vector orientation);
    [[nodiscard]] Vector const& getVelocity() const;
    void setVelocity(Vector velocity);
    [[nodiscard]] Vector const& getAcceleration() const;
    void setAcceleration(Vector acceleration);
    [[nodiscard]] Vector const& getPreviousPosition() const;
    void setPreviousPosition(Vector position);
    [[nodiscard]] Vector const& getForce() const;
    void setForce(Vector in_force);

    [[nodiscard]] ObjectType getObjectType() const;
    void setObjectType(ObjectType in_objectType);

    [[nodiscard]] YAxisOrientation getYAxisOrientation() const;
    void setYAxisOrientation(YAxisOrientation in_orientation);

    friend std::ostream& operator<<(std::ostream& stream, GameObject const& object);

    using Metadata = std::map<std::string, MetadataType>;
    [[nodiscard]] Metadata const& metadata() const;
    void setMetadata(std::string const& in_key, MetadataType const& in_value);

   private:
    static ObjectID nextID;
    static int nextMessageId;
    std::shared_ptr<ObjectData> m_pObjectData;
    ObjectState m_objectState = Active;
    ObjectID m_objectID = -1;
    ObjectID m_parentObjectID = -1;
    ObjectType m_objectType = ObjectType_AI;
    YAxisOrientation m_yAxisOrientation = YAxisOrientation::BottomZero;

    //! metadata
    Metadata m_metadata;

    //! The components
    std::vector<std::shared_ptr<Component>> m_components;

    Vector position;
    Vector previousPosition;
    Vector orientation;
    Vector velocity;
    Vector acceleration;
    Vector force;
};

template <typename T>
std::vector<std::shared_ptr<T>> GameObject::getComponents()
{
    std::vector<std::shared_ptr<T>> components;
    for (auto&& component : m_components) {
        auto castedObject = std::dynamic_pointer_cast<T>(component);
        if (castedObject) {
            components.push_back(castedObject);
        }
    }

    return components;
}

// events
class GameObjectStateChangedEvent : public GameEvent {
   public:
    GameObjectStateChangedEvent(GameObject* object, GameObject::ObjectState stateBefore,
                                GameObject::ObjectState stateAfter);
    ~GameObjectStateChangedEvent() override = default;
    [[nodiscard]] std::string type() const override
    {
        return kType;
    };

    static constexpr auto kType = "GameObjectSateChanged";
    GameObject* m_object;
    GameObject::ObjectState m_stateBefore;
    GameObject::ObjectState m_stateAfter;
};

}  // namespace CapEngine

#endif  // GAMEOBJECT_H
