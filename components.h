#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "camera2d.h"

#include <memory>
#include <optional>

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
    virtual void receive(GameObject* /*object*/, int /*messageId*/, std::string /*message*/) {}
    virtual ComponentType getType() const = 0;

    virtual std::unique_ptr<Component> clone() const = 0;
  };

  class PhysicsComponent : public Component {
  public:
    ~PhysicsComponent() override = default;
    virtual std::optional<Rectangle> boundingPolygon(const GameObject* /*object*/) const { return std::nullopt; }
    ComponentType getType() const override { return ComponentType::Physics; }
  };

  class GraphicsComponent : public Component {
  public:
    ~GraphicsComponent() override = default;
    virtual void render(GameObject* object, const Camera2d &in_camera, uint32_t in_windowId) = 0;
    ComponentType getType() const override { return ComponentType::Graphics; }
  };

  class InputComponent : public Component {
  public:
    ~InputComponent() override = default;
    ComponentType getType() const override { return ComponentType::Input; }
  };

  class CustomComponent : public Component {
  public:
    ~CustomComponent() override = default;
    ComponentType getType() const override { return ComponentType::Custom; }
  };

  class AIComponent : public Component {
  public:
    ~AIComponent() override = default;
    ComponentType getType() const override { return ComponentType::AI; }
  };

}

#endif // COMPONENTS_H
