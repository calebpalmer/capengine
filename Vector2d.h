#ifndef VECTOR2D_H
#define VECTOR2D_H

//@brief - the header file for the Vector2d class
//@author - Caleb Palmer

#include <memory>

#include "CapEngine.h"

namespace CapEngine
{

struct PolarVector;

class Vector2d
{

public:
  Vector2d();
  Vector2d(real x_in, real y_inin = 0);
  Vector2d(const PolarVector &pVecIn);
  Vector2d(const Vector2d &);
  ~Vector2d();
  real getX() const;
  real getY() const;
  void setX(real xIn);
  void setY(real yIn);
  Vector2d operator+(const Vector2d &vec);
  Vector2d operator-(const Vector2d &vec);
  real distance(const Vector2d &vec) const;
  void scale(real factor);
  real magnitude() const;
  Vector2d *normalize() const;
  real dotProduct(const Vector2d &vec) const;
  Vector2d *crossProduct(const Vector2d &vec) const;
  real angle(const Vector2d &vec) const;
  Vector2d *surfaceNormal(const Vector2d &vec) const;
  PolarVector *toPolarVector() const;

private:
  real x;
  real y;

private:
  Vector2d &operator=(const Vector2d &) { return *this; }
};
} // namespace CapEngine
#endif
