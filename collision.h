#ifndef COLLISION_H
#define COLLISION_H

#include "captypes.h"

namespace CapEngine{

  class Rectangle {
  public:
    CapEngine::real x;
    CapEngine::real y;
    CapEngine::real width;
    CapEngine::real height;
    
    Rectangle();
    Rectangle(real xIn,real yIn, real widthIn, real heightIn);
  }; 
  
  enum CollisionType {
    COLLISION_NONE,
    COLLISION_GENERAL,
    COLLISION_ONX,
    COLLISION_ONY,
    COLLISION_LEFT,
    COLLISION_RIGHT,
    COLLISION_TOP,
    COLLISION_BOTTOM
  };

enum CollisionClass{
  COLLISION_WALL,
  COLLISION_PROJECTILE,
  COLLISION_ENTITY
};

 CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2);
 CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2);

}
#endif //COLLISION_H
