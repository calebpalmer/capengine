#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL_keycode.h>

#include <optional>
#include <vector>

#include "captypes.h"
#include "vector.h"

namespace CapEngine {

//! class to  represent a 2D rectangle
class Rectangle {
   public:
    double x;
    double y;
    double width;
    double height;

    Rectangle();
    Rectangle(double xIn, double yIn, double widthIn, double heightIn);
    Rectangle(const Rect& rect);

    friend bool operator==(const Rectangle& in_lhs, const Rectangle& in_rhs);

    Rect toRect() const;

    Rectangle raiseBottom(double in_amount) const;
    Rectangle lowerTop(double in_amount) const;
    Rectangle narrowRight(double in_amount) const;
    Rectangle narrowLeft(double in_amount) const;
};

Rectangle join(const Rectangle& in_left, const Rectangle& in_right);

//! structure to represent a 2D point.
struct Point {
    double x = 0;
    double y = 0;

    bool operator==(const Point&) const = default;
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

enum CollisionClass { COLLISION_UNKNOWN, COLLISION_WALL, COLLISION_PROJECTILE, COLLISION_ENTITY, COLLISION_BITMAP };

enum CollisionVectorType { COLLISIONVECTORTYPE_POINT, COLLISIONVECTORTYPE_LINE };

//! enum for methods to determine the representative point of a collision
enum class RepresentativePointMethod {
    //! Snaps to the side of rect a where the collision first happened.
    Simple,
    //! Calculated a rectangle for the intersection region, returns the center point.
    IntersectionRectangle
};

struct BoxCollision {
    //! The type: top, bottom, left, right
    CollisionType collisionType;
    //! A normal vector for the collision.
    Vector collisionNormal;
    //! A point approximating where the collision first occured.
    Point representativePoint;
};

//! structure reprenting a collision via bitmap detection
struct PixelCollision {
    CollisionType collisionType;
    Vector collisionPoint;
    CollisionVectorType vectorType;
};

bool pointInRect(const Point& point, const Rectangle& rect);

// box collisions
CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2);
std::optional<BoxCollision> detectBoxCollision(
    const Rectangle& a, const Rectangle& b,
    RepresentativePointMethod in_representativePointMethod = RepresentativePointMethod::Simple,
    bool preferLeftRight = true);
std::optional<BoxCollision> detectBoxCollisionWithContactManifold(
    const Rectangle& a, const Rectangle& b,
    RepresentativePointMethod in_representativePointMethod = RepresentativePointMethod::Simple,
    bool preferLeftRight = true);
CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2);
Relation MBRRelate(const Rectangle& r1, const Rectangle& r2);
Relation MBRRelate(int x, int y, const Rectangle& r);

// bitmap collisions
std::vector<std::pair<CollisionType, Vector>> detectBitmapCollision(const Rectangle& rect,
                                                                    const Surface* bitmapSurface);
std::vector<PixelCollision> detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface);
std::vector<PixelCollision> detectBitmapCollisions(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects,
                                                   const Surface* in_bitmapSurface);
std::vector<PixelCollision> detectBitmapCollisionsWithTangents(
    std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, const Surface* in_bitmapSurface);

// resolving collisions
Rectangle resolveInteriorCollision(Rectangle const& r1, Rectangle const& r2, CollisionType collisionType);

// operators
std::ostream& operator<<(std::ostream& stream, const CapEngine::Point& point);
std::ostream& operator<<(std::ostream& stream, const CapEngine::Rectangle& rectangle);
std::ostream& operator<<(std::ostream& stream, const CapEngine::CollisionType& collisionTyoe);
std::ostream& operator<<(std::ostream& stream, const CapEngine::PixelCollision& pixelCollision);
std::ostream& operator<<(std::ostream& stream, const CapEngine::CollisionClass& collisionClass);
}  // namespace CapEngine

#endif // COLLISION_H
