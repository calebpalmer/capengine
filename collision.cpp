#include "collision.h"

#include "scanconvert.h"

using namespace CapEngine;
using namespace std;

Rectangle::Rectangle(){}
Rectangle::Rectangle(real xIn,real yIn, real widthIn, real heightIn) : x(xIn), y(yIn),width(widthIn), height(heightIn) { }

CollisionType CapEngine::detectMBRCollision(const Rectangle& r1, const Rectangle& r2){
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

CollisionType CapEngine::detectMBRCollisionInterior(const Rectangle& r1, const Rectangle& r2){
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


Relation CapEngine::MBRRelate(const Rectangle& r1, const Rectangle& r2) {
  if(detectMBRCollision(r1, r2) != COLLISION_NONE){
    return TOUCH;
  }
  
  if(detectMBRCollisionInterior(r1, r2) != COLLISION_NONE){
    return OUTSIDE;
  }
  
  return INSIDE;
}

Relation CapEngine::MBRRelate(int x, int y, const Rectangle& r){
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
CollisionType CapEngine::detectBitmapCollision(const CapEngine::Rectangle &rect, const Surface *bitmapSurface,
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

vector<PixelCollision> CapEngine::detectBitmapCollisions(const Rectangle& rect, const Surface* bitmapSurface){
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
