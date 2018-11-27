#ifndef NULL_PHYSICS_COMPONENT
#define NULL_PHYSICS_COMPONENT

#include "gameobject.h"

namespace CapEngine {

  class NullPhysicsComponent : public PhysicsComponent {
  public:
    virtual void update(GameObject* /*object*/, double /*timestep*/) {}
    virtual CapEngine::Rectangle  boundingPolygon(const GameObject* /*object*/) const { 
      return CapEngine::Rectangle(0.0, 0.0, 0.0, 0.0);
    }
    virtual bool handleCollision(GameObject*, CapEngine::CollisionType, CapEngine::CollisionClass, GameObject*) {
      return false;
    }
  };

}
#endif
