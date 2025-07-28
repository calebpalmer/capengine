#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <map>
#include <memory>
#include <optional>

#include "camera2d.h"
#include "captypes.h"

namespace CapEngine {
class GameObject;

//! The component type.
enum class ComponentType { Physics, Graphics, Input, Custom, AI };

//! interface class for components.
class Component {
   public:
    virtual ~Component() = default;

    virtual void update(GameObject& object, double timestep) = 0;
    virtual void receive(GameObject& /*object*/, int /*messageId*/, const std::string& /*message*/)
    {
    }
    [[nodiscard]] virtual ComponentType getType() const = 0;

    [[nodiscard]] virtual std::unique_ptr<Component> clone() const = 0;

    //! Metadata collection type
    using Metadata = std::map<std::string, MetadataType>;

    [[nodiscard]] Metadata const& metadata() const;
    void setMetadata(const std::string& in_key, const MetadataType& in_value);

   protected:
    Metadata m_metadata;  //!< Holds metadata about the component.
};

//! Interface class for physics components
class PhysicsComponent : public Component {
   public:
    ~PhysicsComponent() override = default;
    [[nodiscard]] virtual std::optional<Rectangle> boundingPolygon(const GameObject& /*object*/) const
    {
        return std::nullopt;
    }
    virtual bool handleCollision(CollisionType /*in_collisionType*/, CollisionClass /*in_collisionClass*/,
                                 GameObject& /*in_Object*/, std::optional<GameObject*> /*in_OtherObject*/,
                                 const Vector& /*collisionLocation*/)
    {
        return false;
    }
    [[nodiscard]] virtual CollisionType collides([[maybe_unused]] CapEngine::Rectangle const& in_mbr) const
    {
        return CollisionType::COLLISION_NONE;
    }
    [[nodiscard]] ComponentType getType() const override
    {
        return ComponentType::Physics;
    }
};

//! interface class for graphics components
class GraphicsComponent : public Component {
   public:
    ~GraphicsComponent() override = default;
    virtual void render(GameObject& object, const Camera2d& in_camera, uint32_t in_windowId) = 0;
    [[nodiscard]] ComponentType getType() const override
    {
        return ComponentType::Graphics;
    }
};

//! interface class for input components
class InputComponent : public Component {
   public:
    ~InputComponent() override = default;
    [[nodiscard]] ComponentType getType() const override
    {
        return ComponentType::Input;
    }
};

//! interface class for custom components
class CustomComponent : public Component {
   public:
    ~CustomComponent() override = default;
    [[nodiscard]] ComponentType getType() const override
    {
        return ComponentType::Custom;
    }
};

//! interface class for ai components
class AIComponent : public Component {
   public:
    ~AIComponent() override = default;
    [[nodiscard]] ComponentType getType() const override
    {
        return ComponentType::AI;
    }
};

}  // namespace CapEngine

#endif // COMPONENTS_H
