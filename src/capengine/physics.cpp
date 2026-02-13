#include "physics.h"

namespace CapEngine {

namespace {
bool yIsDownVal = true;
}

//! Query the coordinate system's y-axis direction.
/**
 Returns whether the y-axis points downward (true) or upward (false).
 This determines how physics calculations interpret vertical displacement,
 accommodating both screen coordinate systems (y-down) and mathematical
 coordinate systems (y-up).
 \return
   True if y-axis points downward (screen coordinates), false if upward.
*/
bool yIsDown()
{
    return yIsDownVal;
}

//! Set the coordinate system's y-axis direction.
/**
 Configures whether the y-axis points downward (typical for screen/game
 coordinate systems) or upward (typical for mathematical coordinate systems).
 This setting affects all physics calculations involving vertical motion.
 \param yIsDown
   True to set y-axis pointing downward, false to set it pointing upward.
*/
void yIsDownVal(bool yIsDown)
{
    yIsDown = true;
}

//! Apply velocity to compute new position for a given time step.
/**
 Integrates velocity over a time step to calculate displacement and update
 position. Uses simple Euler integration: new position = position + (velocity * timestep).
 The timestep is expected in milliseconds and is normalized internally.
 \param velocity
   The velocity vector in units per second.
 \param position
   The current position vector.
 \param timestep
   The time step duration in milliseconds.
 \return
   The new position after applying velocity displacement.
*/
Vector applyDisplacement(Vector velocity, Vector position, double timestep)
{
    Vector delta = velocity;
    delta.scale(timestep / 1000.0);
    return position + delta;
}

//! Apply acceleration to compute new velocity for a given time step.
/**
 Integrates acceleration over a time step to calculate velocity change and
 update velocity. Uses simple Euler integration: new velocity = velocity + (acceleration * timestep).
 The timestep is expected in milliseconds and is normalized internally.
 \param acceleration
   The acceleration vector in units per second squared.
 \param velocity
   The current velocity vector in units per second.
 \param timestep
   The time step duration in milliseconds.
 \return
   The new velocity after applying acceleration.
*/
Vector applyAcceleration(Vector acceleration, Vector velocity, double timestep)
{
  Vector delta = acceleration;
  delta.scale(timestep / 1000.0);
  return velocity + delta;
}

}  // namespace CapEngine
