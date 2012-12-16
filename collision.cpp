#include "collision.h"

using namespace CapEngine;

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
