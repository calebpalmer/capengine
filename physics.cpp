#include "physics.h"

namespace CapEngine {

Vector applyDisplacement(Vector velocity, Vector position, double timestep){
  Vector delta = velocity;
  delta.scale(timestep / 1000.0);
  return position + delta;
}

Vector applyAcceleration(Vector acceleration, Vector velocity, double timestep){
  Vector delta = acceleration;
  delta.scale(timestep / 1000.0);
  return velocity + delta;
}

} // namespace CapEngine
