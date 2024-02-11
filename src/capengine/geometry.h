/*
  \brief Various geometry related functions
  \author Caleb Palmer
 */

#include "Vector2d.h"

namespace CapEngine
{

// Vectors
bool doVectorsIntersect2d(const Vector2d *vector1, const Vector2d *vector2);
Vector2d *vectorIntersectionPoint2d(const Vector2d *vector1,
									const Vector2d *vector2);
double getDistance(const Vector2d *vector1, const Vector2d *vector2);
Vector2d *midPoint(const Vector2d *vector1, const Vector2d *vector2);

} // namespace CapEngine
