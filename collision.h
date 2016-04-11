#ifndef COLLISION_H
#define COLLISION_H

#include <vector>

#include "captypes.h"
#include "vector.h"

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

  enum Relation {
    OUTSIDE,
    INSIDE,
    TOUCH
  };

  enum CollisionClass{
    COLLISION_UNKNOWN,
    COLLISION_WALL,
    COLLISION_PROJECTILE,
    COLLISION_ENTITY
  };

  struct PixelCollision{
    CollisionType collisionType;
    Vector collisionPoint;
  };


 CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2);
 CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2);
 Relation MBRRelate(const Rectangle& r1, const Rectangle& r2);
 Relation MBRRelate(int x, int y, const Rectangle& r);
 CollisionType detectBitmapCollision(const Rectangle& rect, const Surface* bitmapSurface,
				     Vector& collisionPoint);
 std::vector<PixelCollision> detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface);

}
#endif //COLLISION_H
