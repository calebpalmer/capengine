#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <map>

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
    Rectangle(Rect rect);

    Rect toRect() const;

    Rectangle raiseBottom(CapEngine::real in_amount) const;
    Rectangle lowerTop(CapEngine::real in_amount) const;
    Rectangle narrowRight(CapEngine::real in_amount) const;
    Rectangle narrowLeft(CapEngine::real in_amount) const;
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
    COLLISION_ENTITY,
    COLLISION_BITMAP
  };

  enum CollisionVectorType {
    COLLISIONVECTORTYPE_POINT,
    COLLISIONVECTORTYPE_LINE
  };
  
  struct PixelCollision{
    CollisionType collisionType;
    Vector collisionPoint;
    CollisionVectorType vectorType;
  };


 CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2);
 CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2);
 Relation MBRRelate(const Rectangle& r1, const Rectangle& r2);
 Relation MBRRelate(int x, int y, const Rectangle& r);
 CollisionType detectBitmapCollision(const Rectangle& rect, const Surface* bitmapSurface,
				     Vector& out_collisionPoint);
 std::vector<PixelCollision> detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface);
 std::vector<PixelCollision> detectBitmapCollisions(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, const Surface* in_bitmapSurface);
 std::vector<PixelCollision> detectBitmapCollisionsWithTangents(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, const Surface* in_bitmapSurface);

 std::ostream& operator<<(std::ostream& stream, const CapEngine::CollisionType& collisionTyoe);
 std::ostream& operator<<(std::ostream& stream, const CapEngine::PixelCollision& pixelCollision);
 std::ostream& operator<<(std::ostream& stream, const CapEngine::CollisionClass& collisionClass);
}



#endif //COLLISION_H
