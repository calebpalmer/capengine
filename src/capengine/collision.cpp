#include "collision.h"

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <boost/throw_exception.hpp>
#include <functional>
#include <optional>
#include <sstream>
#include <stdexcept>

#include "locator.h"
#include "logging.h"
#include "physics.h"
#include "scanconvert.h"

using namespace std;

namespace CapEngine {

//! Default constructor.
Rectangle::Rectangle()
{
}

//! Constructor.
/**
 \param xIn
   The x-coordinate.
 \param yIn
   The y-coordinate.
 \param widthIn
   The width.
 \param heightIn
   The height.
*/
Rectangle::Rectangle(int xIn, int yIn, int widthIn, int heightIn) : x(xIn), y(yIn), width(widthIn), height(heightIn)
{
}

//! Constructor from a Rect.
/**
 \param rect
   The Rect object.
*/
Rectangle::Rectangle(const Rect& rect) : x(rect.x), y(rect.y), width(rect.w), height(rect.h)
{
}

//! Converts the Rectangle to a Rect.
/**
 \return
   The Rect object.
*/
Rect Rectangle::toRect() const
{
    Rect rect = {static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), static_cast<int>(std::round(width)),
                 static_cast<int>(std::round(height))};
    return rect;
}

//! Raises the bottom of the rectangle.
/**
 \param in_amount
   The amount to raise the bottom by.
 \return
   The new rectangle.
*/
Rectangle Rectangle::raiseBottom(int in_amount) const
{
    Rectangle newRect = *this;
    newRect.height = newRect.height - in_amount;
    return newRect;
}

//! Lowers the top of the rectangle.
/**
 \param in_amount
   The amount to lower the top by.
 \return
   The new rectangle.
*/
Rectangle Rectangle::lowerTop(int in_amount) const
{
    Rectangle newRect = *this;
    newRect.y = newRect.y + in_amount;
    return newRect;
}

//! Narrows the rectangle from the left.
/**
 \param in_amount
   The amount to narrow by.
 \return
   The new rectangle.
*/
Rectangle Rectangle::narrowLeft(int in_amount) const
{
    Rectangle newRect = *this;
    newRect.x = newRect.x + in_amount;
    return newRect;
}

//! Narrows the rectangle from the right.
/**
 \param in_amount
   The amount to narrow by.
 \return
   The new rectangle.
*/
Rectangle Rectangle::narrowRight(int in_amount) const
{
    Rectangle newRect = *this;
    newRect.width = newRect.width - in_amount;
    return newRect;
}

//! Joins two rectangles together to make a single greater rectangle.
/**
 \param in_left
   The first rectangle.
 \param in_right
   The second rectangle.
 \return
   The joined rectangles.
*/
Rectangle join(const Rectangle& in_left, const Rectangle& in_right)
{
    // calculate x and y
    Rectangle rect{in_left.x <= in_right.x ? in_left.x : in_right.x, in_left.y <= in_right.y ? in_left.y : in_right.y,
                   0, 0};

    // calculate width
    if (in_left.x + in_left.width >= in_right.x + in_right.width) {
        rect.width = in_left.x + in_left.width - rect.x;
    }
    else {
        rect.width = in_right.x + in_right.width - rect.x;
    }

    // calculate height
    if (in_left.y + in_left.height >= in_right.y + in_right.height) {
        rect.height = in_left.y + in_left.height - rect.y;
    }
    else {
        rect.height = in_right.y + in_right.height - rect.y;
    }

    return rect;
}

//! Detects if a point is within a rectangle.
/**
 \param point
   The point.
 \param rect
   The rect.
 \return
   True if point is in rect. False otherwise.
*/
bool pointInRect(const Point& point, const Rectangle& rect)
{
    return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height;
}

//! Detects collision between two minimum bounding rectangles.
/**
 \param r1
   The first rectangle.
 \param r2
   The second rectangle.
 \return
   The type of collision.
*/
CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2)
{
    int top1, top2, bottom1, bottom2, right1, right2, left1, left2;
    left1 = r1.x;
    left2 = r2.x;
    top1 = r1.y;
    top2 = r2.y;
    bottom1 = top1 + r1.height;
    bottom2 = top2 + r2.height;
    right1 = left1 + r1.width;
    right2 = left2 + r2.width;

    if (bottom1 < top2 || bottom2 < top1)
        return COLLISION_NONE;
    if (right1 < left2 || right2 < left1)
        return COLLISION_NONE;

    // objects collided.  What side? TODO
    return COLLISION_GENERAL;
}

//! Detects the side of collision between two rectangles.
/**
 Determines which side of r1 is hit by r2 using minimum overlap detection.
 Computes the overlap distance on all four sides and returns the side with
 the minimum overlap, indicating the collision direction.
 \param r1
   The first rectangle.
 \param r2
   The second rectangle.
 \param in_representativePointMethod
   The algorithm to use for generating a representative point
 \param in_preferLeftRight
   Prefer collisions reported on left or right when it could go either way.
 \return
   The side of r1 where collision occurs (COLLISION_LEFT, COLLISION_RIGHT,
   COLLISION_TOP, or COLLISION_BOTTOM), or COLLISION_NONE if no collision.
*/
std::optional<BoxCollision> detectBoxCollision(const Rectangle& a, const Rectangle& b,
                                               RepresentativePointMethod in_representativePointMethod,
                                               bool in_preferLeftRight)
{
    double aXmin = a.x;
    double aXmax = a.x + a.width;
    double aYmin = a.y;
    double aYmax = a.y + a.height;
    double bXmin = b.x;
    double bXmax = b.x + b.width;
    double bYmin = b.y;
    double bYmax = b.y + b.height;

    bool overlapX = aXmin < bXmax && aXmax > bXmin;
    bool overlapY = aYmin < bYmax && aYmax > bYmin;

    // If there is no overlap on one or both of the axis then
    // there is no collision
    if (!overlapX || not overlapY) {
        return std::nullopt;
    }

    double overlapLeft = bXmax - aXmin;    // b hits a's left
    double overlapRight = aXmax - bXmin;   // b hits a's right
    double overlapBottom = bYmax - aYmin;  // b hits a's bottom
    double overlapTop = aYmax - bYmin;     // b hits as top

    bool yIsDownSetting = yIsDown();
    BoxCollision boxCollision;

    double minOverlap = std::min({overlapLeft, overlapRight, overlapBottom, overlapTop});

    // Figure out first which side has our collision
    if (minOverlap == overlapLeft) {
        boxCollision.collisionType = CollisionType::COLLISION_LEFT;
        boxCollision.collisionNormal = Vector{-1.0};
    }
    if (minOverlap == overlapRight) {
        boxCollision.collisionType = CollisionType::COLLISION_RIGHT;
        boxCollision.collisionNormal = Vector{1.0};
    }
    if (minOverlap == overlapBottom &&
        (!in_preferLeftRight || (minOverlap != overlapLeft && minOverlap != overlapRight))) {
        boxCollision.collisionType = CollisionType::COLLISION_BOTTOM;
        boxCollision.collisionNormal = Vector{0.0, -1.0};
    }
    if (minOverlap == overlapTop &&
        (!in_preferLeftRight || (minOverlap != overlapLeft && minOverlap != overlapRight))) {
        boxCollision.collisionType = CollisionType::COLLISION_TOP;
        boxCollision.collisionNormal = Vector{0.0, 1.0};
    }

    // Figure out a representative point
    if (in_representativePointMethod == RepresentativePointMethod::Simple) {
        Point representativePoint;
        Point centerB{b.x + (b.width / 2.0), b.y + (b.height / 2.0)};
        if (boxCollision.collisionNormal.x == 1) {  // A's right
            representativePoint.x = aXmax;
            representativePoint.y = std::clamp(centerB.y, aYmin, aYmax);
        }
        else if (boxCollision.collisionNormal.x == -1) {  // A's left
            representativePoint.x = aXmin;
            representativePoint.y = std::clamp(centerB.y, aYmin, aYmax);
        }
        else if (boxCollision.collisionNormal.y == 1) {  // A's top
            representativePoint.y = aYmax;
            representativePoint.x = std::clamp(centerB.x, aXmin, aXmax);
        }
        else if (boxCollision.collisionNormal.y == -1) {  // A's bottom
            representativePoint.y = aYmin;
            representativePoint.x = std::clamp(centerB.x, aXmin, aXmax);
        }
        boxCollision.representativePoint = representativePoint;
    }
    else if (in_representativePointMethod == RepresentativePointMethod::IntersectionRectangle) {
        // Create an overlap rectangle so we can choose a representative point of intersection
        float iXmin = max(aXmin, bXmin);
        float iYmin = max(aYmin, bYmin);
        float iXmax = min(aXmax, bXmax);
        float iYmax = min(aYmax, bYmax);

        Point representativePoint{(iXmin + iXmax) * 0.5f, (iYmin + iYmax) * 0.5f};
        boxCollision.representativePoint = representativePoint;
    }

    return boxCollision;
}

std::optional<BoxCollision> detectBoxCollisionWithContactManifold(
    const Rectangle& a, const Rectangle& b, RepresentativePointMethod in_representativePointMethod,
    bool in_preferLeftRight)
{
    // TODO Contact manifold is a more accurate way to perform physics on collisions
    // It returns more than one representative contact point
    BOOST_THROW_EXCEPTION(std::runtime_error{"Not Implemented"});
}

//! Detects if a rectangle is outside another (interior) rectangle.
/**
 \param r1
   The rectangle that should be inside.
 \param r2
   The rectangle that should contain r1.
 \return
   The side of collision if r1 is outside r2, otherwise COLLISION_NONE.
*/
CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2)
{
    // assumes window coordinate system
    if (r1.x < r2.x) {
        return COLLISION_LEFT;
    }
    if (r1.x + r1.width > r2.x + r2.width) {
        return COLLISION_RIGHT;
    }
    if (r1.y < r2.y) {
        return COLLISION_TOP;
    }
    if (r1.y + r1.height > r2.y + r2.height) {
        return COLLISION_BOTTOM;
    }

    return COLLISION_NONE;
}

//! Determines the spatial relation between two rectangles.
/**
 \param r1
   The first rectangle.
 \param r2
   The second rectangle.
 \return
   The relation (TOUCH, OUTSIDE, INSIDE).
*/
Relation MBRRelate(const Rectangle& r1, const Rectangle& r2)
{
    if (detectMBRCollision(r1, r2) != COLLISION_NONE) {
        return TOUCH;
    }

    if (detectMBRCollisionInterior(r1, r2) != COLLISION_NONE) {
        return OUTSIDE;
    }

    return INSIDE;
}

//! Determines the spatial relation of a point to a rectangle.
/**
 \param x
   The x-coordinate of the point.
 \param y
   The y-coordinate of the point.
 \param r
   The rectangle.
 \return
   The relation (INSIDE, OUTSIDE).
*/
Relation MBRRelate(int x, int y, const Rectangle& r)
{
    if ((x >= r.x && x <= r.x + r.width) && (y >= r.y && y <= r.y + r.height)) {
        return INSIDE;
    }
    // should add case for touch at some point
    else {
        return OUTSIDE;
    }
}

// anonymous functions for bitmap collision detection
namespace {

//! Detects a collision with the top part of a rectangle against a bitmap.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The surface of the bitmap.
 \param collisionPoint
   The point of collision (out-parameter).
 \return
   True if a collision was detected, false otherwise.
*/
bool detectTopBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface, Vector& collisionPoint)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    CAP_THROW_NULL(bitmapSurface, "Surface is null");
    CAP_THROW_NULL(Locator::videoManager, "VideoManager not found.");

    const auto width = Locator::videoManager->getSurfaceWidth(bitmapSurface);
    const auto height = Locator::videoManager->getSurfaceHeight(bitmapSurface);

    // start from halfway down rectangle to find lowest rectangle that
    // "top" collides
    int y = rect.y + (rect.height / 2);
    for (; y >= rect.y; y--) {
        for (int x = rect.x; x < rect.x + rect.width; x++) {
            if (y < 0 || y >= height || x < 0 or x >= width)
                continue;

            getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
            if (r == 0x00 && g == 0x00 && b == 0x00) {
                collisionPoint.setX(x);
                collisionPoint.setY(y);
                collisionPoint.setZ(0);
                return true;
            }
        }
    }
    return false;
}

//! Detects a collision with the bottom part of a rectangle against a bitmap.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The surface of the bitmap.
 \param collisionPoint
   The point of collision (out-parameter).
 \return
   True if a collision was detected, false otherwise.
*/
bool detectBottomBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface, Vector& collisionPoint)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    CAP_THROW_NULL(bitmapSurface, "Surface is null");
    CAP_THROW_NULL(Locator::videoManager, "VideoManager not found.");

    const auto width = Locator::videoManager->getSurfaceWidth(bitmapSurface);
    const auto height = Locator::videoManager->getSurfaceHeight(bitmapSurface);

    // start from halfway up to find the first part the collides
    int y = rect.y + (rect.height / 2);
    for (; y <= rect.y + rect.height; y++) {
        for (int x = rect.x; x < rect.x + rect.width; x++) {
            if (y < 0 || y >= height || x < 0 or x >= width)
                continue;

            getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
            if (r == 0x00 && g == 0x00 && b == 0x00) {
                collisionPoint.setX(x);
                collisionPoint.setY(y);
                collisionPoint.setZ(0);
                return true;
            }
        }
    }
    return false;
}

//! Detects a collision with the right part of a rectangle against a bitmap.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The surface of the bitmap.
 \param collisionPoint
   The point of collision (out-parameter).
 \return
   True if a collision was detected, false otherwise.
*/
bool detectRightBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface, Vector& collisionPoint)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    CAP_THROW_NULL(bitmapSurface, "Surface is null");
    CAP_THROW_NULL(Locator::videoManager, "VideoManager not found.");

    const auto width = Locator::videoManager->getSurfaceWidth(bitmapSurface);
    const auto height = Locator::videoManager->getSurfaceHeight(bitmapSurface);

    int x = rect.x + (rect.width / 2);
    for (; x < rect.x + rect.width; x++) {
        for (int y = rect.y; y < rect.y + rect.height; y++) {
            if (y < 0 || y >= height || x < 0 or x >= width)
                continue;

            getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
            if (r == 0x00 && g == 0x00 && b == 0x00) {
                collisionPoint.setX(x);
                collisionPoint.setY(y);
                collisionPoint.setZ(0);
                return true;
            }
        }
    }
    return false;
}

//! Detects a collision with the left part of a rectangle against a bitmap.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The surface of the bitmap.
 \param collisionPoint
   The point of collision (out-parameter).
 \return
   True if a collision was detected, false otherwise.
*/
bool detectLeftBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface, Vector& collisionPoint)
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    CAP_THROW_NULL(bitmapSurface, "Surface is null");
    CAP_THROW_NULL(Locator::videoManager, "VideoManager not found.");

    const auto width = Locator::videoManager->getSurfaceWidth(bitmapSurface);
    const auto height = Locator::videoManager->getSurfaceHeight(bitmapSurface);

    int x = rect.x + (rect.width / 2);
    for (; x >= rect.x; x--) {
        for (int y = rect.y; y < rect.y + rect.height; y++) {
            if (y < 0 || y >= height || x < 0 or x >= width)
                continue;

            getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
            if (r == 0x00 && g == 0x00 && b == 0x00) {
                collisionPoint.setX(x);
                collisionPoint.setY(y);
                collisionPoint.setZ(0);
                return true;
            }
        }
    }
    return false;
}

}  // namespace

//! Detects bitmap collision for a rectangle.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The bitmap surface.
 \return
   A vector of collision types and points.
*/
// assumes 32bpp surface
std::vector<std::pair<CollisionType, Vector>> detectBitmapCollision(const CapEngine::Rectangle& rect,
                                                                    const Surface* bitmapSurface)
{
    std::vector<std::pair<CollisionType, Vector>> collisionTypes;
    Vector collisionPoint;

    // check top collision
    if (detectTopBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        collisionTypes.push_back(std::make_pair(COLLISION_TOP, collisionPoint));
    }

    // check bottom collision
    if (detectBottomBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        collisionTypes.push_back(std::make_pair(COLLISION_BOTTOM, collisionPoint));
    }

    // check left side collition
    if (detectLeftBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        collisionTypes.push_back(std::make_pair(COLLISION_LEFT, collisionPoint));
    }

    // check left side collition
    if (detectRightBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        collisionTypes.push_back(std::make_pair(COLLISION_RIGHT, collisionPoint));
    }

    return collisionTypes;
}

//! Detects bitmap collisions for a rectangle.
/**
 \param rect
   The rectangle to check.
 \param bitmapSurface
   The bitmap surface.
 \return
   A vector of pixel collisions.
*/
vector<PixelCollision> detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface)
{
    vector<PixelCollision> pixelCollisions;
    Vector collisionPoint;

    // check bottom collision
    if (detectBottomBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        PixelCollision pixelCollision;
        pixelCollision.collisionType = COLLISION_BOTTOM;
        pixelCollision.collisionPoint = collisionPoint;
        pixelCollisions.push_back(pixelCollision);
    }

    // check top collision
    if (detectTopBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        PixelCollision pixelCollision;
        pixelCollision.collisionType = COLLISION_TOP;
        pixelCollision.collisionPoint = collisionPoint;
        pixelCollisions.push_back(pixelCollision);
    }

    // check left side collition
    if (detectLeftBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        PixelCollision pixelCollision;
        pixelCollision.collisionType = COLLISION_LEFT;
        pixelCollision.collisionPoint = collisionPoint;
        pixelCollisions.push_back(pixelCollision);
    }

    // check left side collition
    if (detectRightBitmapCollision(rect, bitmapSurface, collisionPoint)) {
        PixelCollision pixelCollision;
        pixelCollision.collisionType = COLLISION_RIGHT;
        pixelCollision.collisionPoint = collisionPoint;
        pixelCollisions.push_back(pixelCollision);
    }

    return pixelCollisions;
}

//! Detects bitmap collisions for a set of rectangles.
/**
 \param in_rects
   The rectangles to check, paired with a collision type hint.
 \param in_bitmapSurface
   The bitmap surface.
 \return
   A vector of pixel collisions.
*/
std::vector<PixelCollision> detectBitmapCollisions(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects,
                                                   const Surface* in_bitmapSurface)
{
    std::vector<PixelCollision> pixelCollisions;

    Vector collisionPoint;
    PixelCollision pixelCollision;

    auto addCollision = [&](CollisionType type) {
        pixelCollision.collisionType = type;
        pixelCollision.collisionPoint = collisionPoint;
        pixelCollision.vectorType = COLLISIONVECTORTYPE_POINT;
        pixelCollisions.push_back(pixelCollision);
    };

    for (auto&& i : in_rects) {
        CollisionType collisionType = i.first;
        Rectangle rect = i.second;

        switch (collisionType) {
            case COLLISION_BOTTOM:
                if (detectBottomBitmapCollision(rect, in_bitmapSurface, collisionPoint))
                    addCollision(collisionType);
                break;
            case COLLISION_TOP:
                if (detectTopBitmapCollision(rect, in_bitmapSurface, collisionPoint))
                    addCollision(collisionType);
                break;
            case COLLISION_LEFT:
                if (detectLeftBitmapCollision(rect, in_bitmapSurface, collisionPoint))
                    addCollision(collisionType);
                break;
            case COLLISION_RIGHT:
                if (detectRightBitmapCollision(rect, in_bitmapSurface, collisionPoint))
                    addCollision(collisionType);
                break;
            default: {
                std::ostringstream msg;
                msg << "Unsupported Collision Type \"" << collisionType << "\"";
                BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::warning) << msg.str();
            }
        }
    }

    return pixelCollisions;
}

//! Detects bitmap collisions and calculates tangents.
/**
 \param in_rects
   The rectangles to check.
 \param in_bitmapSurface
   The bitmap surface.
 \return
   A vector of pixel collisions with tangents.
*/
std::vector<PixelCollision> detectBitmapCollisionsWithTangents(
    std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, const Surface* in_bitmapSurface)
{
    std::vector<PixelCollision> pixelCollisions = detectBitmapCollisions(in_rects, in_bitmapSurface);

    auto findPredicate = [](const CollisionType in_collisionType, const PixelCollision& in_pixelCollision) {
        return in_collisionType == in_pixelCollision.collisionType;
    };

    auto topCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(),
                                     std::bind(findPredicate, COLLISION_TOP, std::placeholders::_1));
    auto bottomCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(),
                                        std::bind(findPredicate, COLLISION_BOTTOM, std::placeholders::_1));
    auto rightCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(),
                                       std::bind(findPredicate, COLLISION_RIGHT, std::placeholders::_1));
    auto leftCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(),
                                      std::bind(findPredicate, COLLISION_LEFT, std::placeholders::_1));

    std::vector<PixelCollision> pixelCollisionWithTangents;

    // bottom
    if (bottomCollision != pixelCollisions.end() && rightCollision != pixelCollisions.end()) {
        Vector tangent = rightCollision->collisionPoint - bottomCollision->collisionPoint;
        PixelCollision collision = {COLLISION_BOTTOM, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    if (bottomCollision != pixelCollisions.end() && leftCollision != pixelCollisions.end()) {
        Vector tangent = leftCollision->collisionPoint - bottomCollision->collisionPoint;
        PixelCollision collision = {COLLISION_BOTTOM, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    // top
    if (topCollision != pixelCollisions.end() && rightCollision != pixelCollisions.end()) {
        Vector tangent = rightCollision->collisionPoint - topCollision->collisionPoint;
        PixelCollision collision = {COLLISION_TOP, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    if (topCollision != pixelCollisions.end() && leftCollision != pixelCollisions.end()) {
        Vector tangent = leftCollision->collisionPoint - topCollision->collisionPoint;
        PixelCollision collision = {COLLISION_TOP, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    // right
    if (rightCollision != pixelCollisions.end() && topCollision != pixelCollisions.end()) {
        Vector tangent = topCollision->collisionPoint - rightCollision->collisionPoint;
        PixelCollision collision = {COLLISION_RIGHT, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    if (rightCollision != pixelCollisions.end() && bottomCollision != pixelCollisions.end()) {
        Vector tangent = bottomCollision->collisionPoint - rightCollision->collisionPoint;
        PixelCollision collision = {COLLISION_RIGHT, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    // left
    if (leftCollision != pixelCollisions.end() && topCollision != pixelCollisions.end()) {
        Vector tangent = topCollision->collisionPoint - leftCollision->collisionPoint;
        PixelCollision collision = {COLLISION_LEFT, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    if (leftCollision != pixelCollisions.end() && bottomCollision != pixelCollisions.end()) {
        Vector tangent = bottomCollision->collisionPoint - leftCollision->collisionPoint;
        PixelCollision collision = {COLLISION_LEFT, tangent, COLLISIONVECTORTYPE_LINE};
        pixelCollisionWithTangents.push_back(collision);
    }

    return pixelCollisionWithTangents;
}

//! Overload for streaming CollisionType.
/**
 \param stream
   The output stream.
 \param collisionType
   The collision type.
 \return
   The output stream.
*/
std::ostream& operator<<(std::ostream& stream, const CollisionType& collisionType)
{
    using namespace CapEngine;

    std::string repr;

    switch (collisionType) {
        case COLLISION_NONE:
            repr = "COLLISION_NONE";
            break;
        case COLLISION_GENERAL:
            repr = "COLLISION_GENERAL";
            break;
        case COLLISION_ONX:
            repr = "COLLISION_ONX";
            break;
        case COLLISION_ONY:
            repr = "COLLISION_ONY";
            break;
        case COLLISION_LEFT:
            repr = "COLLISION_LEFT";
            break;
        case COLLISION_RIGHT:
            repr = "COLLISION_RIGHT";
            break;
        case COLLISION_TOP:
            repr = "COLLISION_TOP";
            break;
        case COLLISION_BOTTOM:
            repr = "COLLISION_BOTTOM";
            break;
    }
    stream << repr;
    return stream;
}

//! Overload for streaming PixelCollision.
/**
 \param stream
   The output stream.
 \param pixelCollision
   The pixel collision object.
 \return
   The output stream.
*/
std::ostream& operator<<(std::ostream& stream, const PixelCollision& pixelCollision)
{
    std::ostringstream repr;
    repr << pixelCollision.collisionType << " @ " << pixelCollision.collisionPoint;

    stream << repr.str();
    return stream;
}

//! Overload for streaming CollisionClass.
/**
 \param stream
   The output stream.
 \param collisionClass
   The collision class.
 \return
   The output stream.
*/
std::ostream& operator<<(std::ostream& stream, const CollisionClass& collisionClass)
{
    using namespace CapEngine;

    std::string repr;

    switch (collisionClass) {
        case COLLISION_UNKNOWN:
            repr = "COLLISION_UNKNOWN";
            break;
        case COLLISION_WALL:
            repr = "COLLISION_WALL";
            break;
        case COLLISION_PROJECTILE:
            repr = "COLLISION_PROJECTILE";
            break;
        case COLLISION_ENTITY:
            repr = "COLLISION_ENTITY";
            break;
        case COLLISION_BITMAP:
            repr = "COLLISION_BITMAP";
            break;
    }

    stream << repr;
    return stream;
}

//! Overload for streaming Point.
/**
 \param stream
   The output stream.
 \param point
   The point object.
 \return
   The output stream.
*/
std::ostream& operator<<(std::ostream& stream, const Point& point)
{
    stream << "Point{" << point.x << ", " << point.y << "}";
    return stream;
}

//! Overload for streaming Rectangle.
/**
 \param stream
   The output stream.
 \param rectangle
   The rectangle object.
 \return
   The output stream.
*/
std::ostream& operator<<(std::ostream& stream, const Rectangle& rectangle)
{
    using namespace CapEngine;

    stream << "Rectangle (" << rectangle.x << ", " << rectangle.y << ", " << rectangle.width << " x "
           << rectangle.height << ")";

    return stream;
}

//! Equality operator for Rectangle.
/**
 \param in_lhs
   The left-hand side rectangle.
 \param in_rhs
   The right-hand side rectangle.
 \return
   True if the rectangles are equal, false otherwise.
*/
bool operator==(const Rectangle& in_lhs, const Rectangle& in_rhs)
{
    return in_lhs.x == in_rhs.x && in_lhs.y == in_rhs.y && in_lhs.width == in_rhs.width &&
           in_lhs.height == in_rhs.height;
}

}  // namespace CapEngine
