#ifndef COLLISION_H
#define COLLISION_H

#include <map>
#include <vector>

#include "SDL_keycode.h"
#include "captypes.h"
#include "vector.h"

namespace CapEngine
{

//! class to  represent a 2D rectangle
class Rectangle
{
public:
  int x;
  int y;
  int width;
  int height;

  Rectangle();
  Rectangle(int xIn, int yIn, int widthIn, int heightIn);
  Rectangle(const Rect &rect);

  friend bool operator==(const Rectangle &in_lhs, const Rectangle &in_rhs);

  Rect toRect() const;

  Rectangle raiseBottom(int in_amount) const;
  Rectangle lowerTop(int in_amount) const;
  Rectangle narrowRight(int in_amount) const;
  Rectangle narrowLeft(int in_amount) const;
};

Rectangle join(const Rectangle &in_left, const Rectangle &in_right);

//! structure to represent a 2D point.
struct Point {
  int x;
  int y;

  Point(int in_x, int in_y) : x(in_x), y(in_y) {}

  Point(std::pair<int, int> point) : x(point.first), y(point.second) {}
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

enum Relation { OUTSIDE, INSIDE, TOUCH };

enum CollisionClass {
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

struct PixelCollision {
  CollisionType collisionType;
  Vector collisionPoint;
  CollisionVectorType vectorType;
};

bool pointInRect(const Point &point, const Rectangle &rect);
CollisionType detectMBRCollision(const Rectangle &r1, const Rectangle &r2);
CollisionType detectMBRCollisionInterior(const Rectangle &r1,
                                         const Rectangle &r2);
Relation MBRRelate(const Rectangle &r1, const Rectangle &r2);
Relation MBRRelate(int x, int y, const Rectangle &r);
std::vector<std::pair<CollisionType, Vector>>
    detectBitmapCollision(const Rectangle &rect, const Surface *bitmapSurface);
std::vector<PixelCollision>
    detectBitmapCollisions(const Rectangle &rect, const Surface *bitmapSurface);
std::vector<PixelCollision> detectBitmapCollisions(
    std::vector<std::pair<CollisionType, Rectangle>> const &in_rects,
    const Surface *in_bitmapSurface);
std::vector<PixelCollision> detectBitmapCollisionsWithTangents(
    std::vector<std::pair<CollisionType, Rectangle>> const &in_rects,
    const Surface *in_bitmapSurface);

std::ostream &operator<<(std::ostream &stream,
                         const CapEngine::Rectangle &rectangle);
std::ostream &operator<<(std::ostream &stream,
                         const CapEngine::CollisionType &collisionTyoe);
std::ostream &operator<<(std::ostream &stream,
                         const CapEngine::PixelCollision &pixelCollision);
std::ostream &operator<<(std::ostream &stream,
                         const CapEngine::CollisionClass &collisionClass);
} // namespace CapEngine

#endif // COLLISION_H
