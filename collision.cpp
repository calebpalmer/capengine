#include "collision.h"

using namespace CapEngine;

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
