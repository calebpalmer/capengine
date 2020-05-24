#ifndef NULL_PHYSICS_COMPONENT
#define NULL_PHYSICS_COMPONENT

#include "gameobject.h"

namespace CapEngine {

  class NullPhysicsComponent : public PhysicsComponent {
  public:
    virtual void update(GameObject* /*object*/, double /*timestep*/) {}
    virtual std::optional<Rectangle>  boundingPolygon(const GameObject& /*object*/) const override { 
      return CapEngine::Rectangle(0.0, 0.0, 0.0, 0.0);
    }
  };

}
#endif
