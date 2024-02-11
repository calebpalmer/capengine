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
  Vector2d(double x_in, double y_inin = 0);
  Vector2d(const PolarVector &pVecIn);
  Vector2d(const Vector2d &);
  ~Vector2d();
  double getX() const;
  double getY() const;
  void setX(double xIn);
  void setY(double yIn);
  Vector2d operator+(const Vector2d &vec);
  Vector2d operator-(const Vector2d &vec);
  double distance(const Vector2d &vec) const;
  void scale(double factor);
  double magnitude() const;
  Vector2d *normalize() const;
  double dotProduct(const Vector2d &vec) const;
  Vector2d *crossProduct(const Vector2d &vec) const;
  double angle(const Vector2d &vec) const;
  Vector2d *surfaceNormal(const Vector2d &vec) const;
  PolarVector *toPolarVector() const;

private:
  double x;
  double y;

private:
  Vector2d &operator=(const Vector2d &) { return *this; }
};
} // namespace CapEngine
#endif
