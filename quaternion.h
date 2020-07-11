#ifndef CAPENGINE_QUATERNIAN_H
#define CAPENGINE_QUATERNIAN_H

#include "vector.h"

namespace CapEngine
{

//! Quaternion class for managing angular displacement
class Quaternion
{
public:
  Quaternion(const Vector &v, double angle);
  Quaternion(double w, double x, double y, double z);

  double getW() const;
  double getX() const;
  double getY() const;
  double getZ() const;

  Quaternion negate() const;
  Quaternion conjugate() const;
  Quaternion inverse() const;
  double magnitude() const;

  bool operator==(const Quaternion &q2) const;
  Quaternion operator*(const Quaternion &q2) const;

protected:
  double m_w = 0;
  double m_x = 0;
  double m_y = 0;
  double m_z = 0;
};

} // namespace CapEngine

#endif // CAPENGINE_QUATERNIAN_H
