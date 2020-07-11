#include "collision.h"

#include "scanconvert.h"
#include "locator.h"
#include "logger.h"

#include <sstream>
#include <algorithm>
#include <functional>

using namespace std;

namespace CapEngine{

  Rectangle::Rectangle(){}
  Rectangle::Rectangle(int xIn,int yIn, int widthIn, int heightIn) : x(xIn), y(yIn),width(widthIn), height(heightIn) { }
  Rectangle::Rectangle(const Rect &rect) : x(rect.x), y(rect.y), width(rect.w), height(rect.h) {}

  Rect Rectangle::toRect() const{
    Rect rect = {static_cast<int>(std::round(x)),
		 static_cast<int>(std::round(y)),
		 static_cast<int>(std::round(width)),
		 static_cast<int>(std::round(height))};
    return rect;
  }

  Rectangle Rectangle::raiseBottom(int in_amount) const{
    Rectangle newRect = *this;
    newRect.height = newRect.height - in_amount;
    return newRect;
  }

  Rectangle Rectangle::lowerTop(int in_amount) const{
    Rectangle newRect = *this;
    newRect.y = newRect.y + in_amount;
    return newRect;
  }

  Rectangle Rectangle::narrowLeft(int in_amount) const{
    Rectangle newRect = *this;
    newRect.x = newRect.x + in_amount;
    return newRect;
  }

  Rectangle Rectangle::narrowRight(int in_amount) const{
    Rectangle newRect = *this;
    newRect.width = newRect.width - in_amount;
    return newRect;
  }

  //! joins to rectangles together to make a single greater rectangle.
  /** 
      \param left
      The first rectangle
      \param right
      The second rectangle
      \return 
      The joined rectangles.
  */
  Rectangle join(const Rectangle &in_left, const Rectangle &in_right){
    // calculate x and y
    Rectangle rect{
		   in_left.x <= in_right.x ? in_left.x : in_right.x,
		   in_left.y <= in_right.y ? in_left.y : in_right.y,
		   0,0
    };

    // calculate width
    if(in_left.x + in_left.width >= in_right.x + in_right.width){
      rect.width = in_left.x + in_left.width - rect.x;
    }
    else{
      rect.width = in_right.x + in_right.width - rect.x;
    }

    // calculate height
    if(in_left.y + in_left.height >= in_right.y + in_right.height){
      rect.height = in_left.y + in_left.height - rect.y;
    }
    else{
      rect.height = in_right.y + in_right.height - rect.y;
    }

    return rect;
  }


  //! detects if point is within a rectangle.
  /** 
      \param point - The point.
      \param rect - the rect.
      \return True if point is in rect. False otherwise.
  */
  bool pointInRect(const Point &point, const Rectangle& rect){
    return point.x >= rect.x &&
      point.x <= rect.x + rect.width &&
      point.y >= rect.y &&
      point.y <= rect.y + rect.height;
  }

  CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2){
    int top1, top2, bottom1, bottom2, right1, right2, left1, left2;
    left1 = r1.x;
    left2 = r2.x;
    top1 = r1.y;
    top2 = r2.y;
    bottom1 = top1 + r1.height;
    bottom2 = top2 + r2.height;
    right1 = left1 + r1.width;
    right2 = left2 + r2.width;

    if(bottom1 < top2 || bottom2 < top1)
      return COLLISION_NONE;
    if(right1 < left2 || right2 < left1)
      return COLLISION_NONE;

    //objects collided.  What side? TODO
    return COLLISION_GENERAL;
  
  }

  CollisionType detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2){
    // assumes window coordinate system
    if(r1.x < r2.x){
      return COLLISION_LEFT;
    }
    if(r1.x + r1.width > r2.x + r2.width){
      return COLLISION_RIGHT;
    }
    if(r1.y < r2.y){
      return COLLISION_TOP;
    }
    if(r1.y + r1.height > r2.y + r2.height){
      return COLLISION_BOTTOM;
    }

    return COLLISION_NONE;
  
  }


  Relation MBRRelate(const Rectangle& r1, const Rectangle& r2) {
    if(detectMBRCollision(r1, r2) != COLLISION_NONE){
      return TOUCH;
    }
  
    if(detectMBRCollisionInterior(r1, r2) != COLLISION_NONE){
      return OUTSIDE;
    }
  
    return INSIDE;
  }

  Relation MBRRelate(int x, int y, const Rectangle& r){
    if((x >= r.x && x <= r.x + r.width) && (y >= r.y && y <= r.y + r.height)){
      return INSIDE;
    }
    // should add case for touch at some point
    else{
      return OUTSIDE;
    }
  }

  // anonymous functions for bitmap collision detection
  namespace {
  
    bool detectTopBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface,
				  Vector& collisionPoint){
      Uint8 r;
      Uint8 g;
      Uint8  b;
      Uint8  a;
      // start from halfway down rectangle to find lowest rectangle that
      // "top" collides
      int y = rect.y + (rect.height / 2);
      for( ; y > rect.y; y --){
	for(int x = rect.x; x < rect.x + rect.width - 1; x++){
	  getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
	  if(r == 0x00 && g == 0x00 && b == 0x00){
	    collisionPoint.setX(x);
	    collisionPoint.setY(y);
	    collisionPoint.setZ(0);
	    return true;
	  }
	}
      }
      return false;
    }

    bool detectBottomBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface,
				     Vector& collisionPoint){
      Uint8 r;
      Uint8 g;
      Uint8  b;
      Uint8  a;

      // start from halfway up to find the first part the collides
      int y = rect.y + (rect.height / 2);
      for(; y < rect.y + rect.height; y++){
	for(int x = rect.x; x < rect.x + rect.width - 1; x++){
	  getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
	  if(r == 0x00 && g == 0x00 && b == 0x00){
	    collisionPoint.setX(x);
	    collisionPoint.setY(y);
	    collisionPoint.setZ(0);
	    return true;
	  }
	}
      }
      return false;
    }

    bool detectRightBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface,
				    Vector& collisionPoint){
      Uint8 r;
      Uint8 g;
      Uint8  b;
      Uint8  a;

      int x = rect.x + (rect.width / 2);
      for(; x < rect.x + rect.width; x++){
	for(int y = rect.y; y < rect.y + rect.height - 1; y++){
	  getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
	  if(r == 0x00 && g == 0x00 && b == 0x00){
	    collisionPoint.setX(x);
	    collisionPoint.setY(y);
	    collisionPoint.setZ(0);
	    return true;
	  }
	}
      }
      return false;
    }

    bool detectLeftBitmapCollision(const CapEngine::Rectangle& rect, const Surface* bitmapSurface,
				   Vector& collisionPoint){
      Uint8 r;
      Uint8 g;
      Uint8  b;
      Uint8  a;

      int x = rect.x + (rect.width / 2);
      for(; x > rect.x; x--){
	for(int y = rect.y; y < rect.y + rect.height - 1; y++){
	  getPixelComponents(bitmapSurface, x, y, &r, &g, &b, &a);
	  if(r == 0x00 && g == 0x00 && b == 0x00){
	    collisionPoint.setX(x);
	    collisionPoint.setY(y);
	    collisionPoint.setZ(0);
	    return true;
	  }
	}
      }
      return false;
    }

  }

  // assumes 32bpp surface
  CollisionType detectBitmapCollision(const CapEngine::Rectangle &rect, const Surface *bitmapSurface,
				      Vector& collisionPoint){

    // check top collision
    if(detectTopBitmapCollision(rect, bitmapSurface, collisionPoint)){
      return COLLISION_TOP;
    }
  
    // check bottom collision
    if(detectBottomBitmapCollision(rect, bitmapSurface, collisionPoint)){
      return COLLISION_BOTTOM;
    }

    // check left side collition
    if(detectLeftBitmapCollision(rect, bitmapSurface, collisionPoint)){
      return COLLISION_LEFT;
    }

    // check left side collition
    if(detectRightBitmapCollision(rect, bitmapSurface, collisionPoint)){
      return COLLISION_RIGHT;
    }

    return COLLISION_NONE;
  }

  vector<PixelCollision> detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface){

    vector<PixelCollision> pixelCollisions;
    Vector collisionPoint;

    // check bottom collision
    if(detectBottomBitmapCollision(rect, bitmapSurface, collisionPoint)){
      PixelCollision pixelCollision;
      pixelCollision.collisionType = COLLISION_BOTTOM;
      pixelCollision.collisionPoint = collisionPoint;
      pixelCollisions.push_back(pixelCollision);
    }

    // check top collision
    if(detectTopBitmapCollision(rect, bitmapSurface, collisionPoint)){
      PixelCollision pixelCollision;
      pixelCollision.collisionType = COLLISION_TOP;
      pixelCollision.collisionPoint = collisionPoint;
      pixelCollisions.push_back(pixelCollision);
    }

    // check left side collition
    if(detectLeftBitmapCollision(rect, bitmapSurface, collisionPoint)){
      PixelCollision pixelCollision;
      pixelCollision.collisionType = COLLISION_LEFT;
      pixelCollision.collisionPoint = collisionPoint;
      pixelCollisions.push_back(pixelCollision);
    }

    // check left side collition
    if(detectRightBitmapCollision(rect, bitmapSurface, collisionPoint)){
      PixelCollision pixelCollision;
      pixelCollision.collisionType = COLLISION_RIGHT;
      pixelCollision.collisionPoint = collisionPoint;
      pixelCollisions.push_back(pixelCollision);
    }

    return pixelCollisions;

  }

  std::vector<PixelCollision> detectBitmapCollisions(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, 
						     const Surface* in_bitmapSurface){
    std::vector<PixelCollision> pixelCollisions;

    Vector collisionPoint;
    PixelCollision pixelCollision;

    auto addCollision = [&](CollisionType type){
			  pixelCollision.collisionType = type;
			  pixelCollision.collisionPoint = collisionPoint;
			  pixelCollision.vectorType = COLLISIONVECTORTYPE_POINT;
			  pixelCollisions.push_back(pixelCollision);
			};

    for (auto && i : in_rects){
      CollisionType collisionType = i.first;
      Rectangle rect = i.second;

      switch(collisionType){
      case COLLISION_BOTTOM:
	if(detectBottomBitmapCollision(rect, in_bitmapSurface, collisionPoint)) 
	  addCollision(collisionType);
	break;
      case COLLISION_TOP:
	if(detectTopBitmapCollision(rect, in_bitmapSurface, collisionPoint)) 
	  addCollision(collisionType);
	break;
      case COLLISION_LEFT:
	if(detectLeftBitmapCollision(rect, in_bitmapSurface, collisionPoint)) 
	  addCollision(collisionType);
	break;
      case COLLISION_RIGHT:
	if(detectRightBitmapCollision(rect, in_bitmapSurface, collisionPoint)) 
	  addCollision(collisionType);
	break;
      default:
	{
	  std::ostringstream msg;
	  msg << "Unsupported Collision Type \"" <<  collisionType << "\"";
	  Locator::logger->log(msg.str(), Logger::CWARNING, __FILE__, __LINE__); 
	}
      }
    }

    return pixelCollisions;
  }

  std::vector<PixelCollision> detectBitmapCollisionsWithTangents(std::vector<std::pair<CollisionType, Rectangle>> const& in_rects, 
								 const Surface* in_bitmapSurface){
    std::vector<PixelCollision> pixelCollisions = detectBitmapCollisions(in_rects, in_bitmapSurface);

    auto findPredicate = [](const CollisionType in_collisionType, const PixelCollision& in_pixelCollision){
			   return in_collisionType == in_pixelCollision.collisionType;
			 };

    auto topCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(), std::bind(findPredicate, COLLISION_TOP, std::placeholders::_1));
    auto bottomCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(), std::bind(findPredicate, COLLISION_BOTTOM, std::placeholders::_1));
    auto rightCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(), std::bind(findPredicate, COLLISION_RIGHT, std::placeholders::_1));        
    auto leftCollision = std::find_if(pixelCollisions.begin(), pixelCollisions.end(), std::bind(findPredicate, COLLISION_LEFT, std::placeholders::_1));

    std::vector<PixelCollision> pixelCollisionWithTangents;
    
    // bottom 
    if(bottomCollision != pixelCollisions.end() && rightCollision != pixelCollisions.end()){
      Vector tangent = rightCollision->collisionPoint - bottomCollision->collisionPoint;
      PixelCollision collision = { COLLISION_BOTTOM, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    if(bottomCollision != pixelCollisions.end() && leftCollision != pixelCollisions.end()){
      Vector tangent = leftCollision->collisionPoint - bottomCollision->collisionPoint;
      PixelCollision collision = { COLLISION_BOTTOM, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    // top
    if(topCollision != pixelCollisions.end() && rightCollision != pixelCollisions.end()){
      Vector tangent = rightCollision->collisionPoint - topCollision->collisionPoint;
      PixelCollision collision = { COLLISION_TOP, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    if(topCollision != pixelCollisions.end() && leftCollision != pixelCollisions.end()){
      Vector tangent = leftCollision->collisionPoint - topCollision->collisionPoint;
      PixelCollision collision = { COLLISION_TOP, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    // right
    if(rightCollision != pixelCollisions.end() && topCollision != pixelCollisions.end()){
      Vector tangent = topCollision->collisionPoint - rightCollision->collisionPoint;
      PixelCollision collision = { COLLISION_RIGHT, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    if(rightCollision != pixelCollisions.end() && bottomCollision != pixelCollisions.end()){
      Vector tangent = bottomCollision->collisionPoint - rightCollision->collisionPoint;
      PixelCollision collision = { COLLISION_RIGHT, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    // left
    if(leftCollision != pixelCollisions.end() && topCollision != pixelCollisions.end()){
      Vector tangent = topCollision->collisionPoint - leftCollision->collisionPoint;
      PixelCollision collision = { COLLISION_LEFT, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }

    if(leftCollision != pixelCollisions.end() && bottomCollision != pixelCollisions.end()){
      Vector tangent = bottomCollision->collisionPoint - leftCollision->collisionPoint;
      PixelCollision collision = { COLLISION_LEFT, tangent, COLLISIONVECTORTYPE_LINE };
      pixelCollisionWithTangents.push_back(collision);
    }
    

    return pixelCollisionWithTangents;
  }

  std::ostream& operator<<(std::ostream& stream, const CollisionType& collisionType){
    using namespace CapEngine;
  
    std::string repr;

    switch(collisionType){
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

  std::ostream& operator<<(std::ostream& stream, const PixelCollision& pixelCollision){
    std::ostringstream repr;
    repr << pixelCollision.collisionType << " @ " << pixelCollision.collisionPoint;

    stream << repr.str();
    return stream;
  }

  std::ostream& operator<<(std::ostream& stream, const CollisionClass& collisionClass){
    using namespace CapEngine;

    std::string repr;

    switch(collisionClass){
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

  std::ostream& operator<<(std::ostream& stream, const Rectangle& rectangle){
    using namespace CapEngine;

    stream << "Rectangle (" << rectangle.x << ", " << rectangle.y << ", " <<
      rectangle.width << " x " << rectangle.height << ")";

    return stream;
  }

  bool operator==(const Rectangle &in_lhs, const Rectangle &in_rhs){
    return in_lhs.x == in_rhs.x &&
      in_lhs.y == in_rhs.y &&
      in_lhs.width == in_rhs.width &&
      in_lhs.height == in_rhs.height;
  }

}
