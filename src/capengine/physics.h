#ifndef PHYSICS_H
#define PHYSICS_H

#include "gameobject.h"
#include "vector.h"

namespace CapEngine
{
Vector applyDisplacement(Vector velocity, Vector position, double timestep);
Vector applyAcceleration(Vector acceleration, Vector velociy, double timestep);
} // namespace CapEngine

#endif // PHYSICS_H
