#ifndef COLLISION_H
#define COLLISION_H

#include "CapEngine.h"

namespace CapEngine{

struct Rectangle {
  CapEngine::real x;
  CapEngine::real y;
  CapEngine::real width;
  CapEngine::real height;
}; 

enum CollisionType {
  COLLISION_NONE,
  COLLISION_GENERAL,
  COLLISION_ONX,
  COLLISION_ONY
};

CollisionType detectMBRCollision(const Rectangle& r1, const Rectangle& r2);

}
#endif //COLLISION_H
