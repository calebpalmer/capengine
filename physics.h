#ifndef PHYSICS_H
#define PHYSICS_H

#include "vector.h"
#include "gameobject.h"

namespace CapEngine{
  Vector applyDisplacement(Vector velocity, Vector position, double timestep);
  Vector applyAcceleration(Vector acceleration, Vector velociy, double timestep);
}

#endif // PHYSICS_H
