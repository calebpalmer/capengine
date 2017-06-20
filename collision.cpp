#include "collision.h"

#include "scanconvert.h"

#include <sstream>

using namespace std;

namespace CapEngine{

  Rectangle::Rectangle(){}
  Rectangle::Rectangle(real xIn,real yIn, real widthIn, real heightIn) : x(xIn), y(yIn),width(widthIn), height(heightIn) { }
  Rectangle::Rectangle(Rect rect) : x(rect.x), y(rect.y), width(rect.w), height(rect.h) {}

  Rect Rectangle::toRect() const{
    Rect rect = {static_cast<int>(std::round(x)),
		 static_cast<int>(std::round(y)),
		 static_cast<int>(std::round(width)),
		 static_cast<int>(std::round(height))};
    return rect;
  }

  CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2){
    real top1, top2, bottom1, bottom2, right1, right2, left1, left2;
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
	for(int x = rect.x; x < rect.x + rect.width; x++){
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
	for(int x = rect.x; x < rect.x + rect.width; x++){
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
	for(int y = rect.y; y < rect.y + rect.height; y++){
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
	for(int y = rect.y; y < rect.y + rect.height; y++){
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
}
