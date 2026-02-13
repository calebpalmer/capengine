#ifndef PHYSICS_H
#define PHYSICS_H

#include "vector.h"

namespace CapEngine {
bool yIsDown();
void yisDown(bool yIsDown = true);
Vector applyDisplacement(Vector velocity, Vector position, double timestep);
Vector applyAcceleration(Vector acceleration, Vector velociy, double timestep);
}  // namespace CapEngine

#endif // PHYSICS_H
