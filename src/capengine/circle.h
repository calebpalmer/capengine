#ifndef CIRCLE_H
#define CIRCLE_H

/*
  \brief Header file for circle class
  \author Caleb Palmer
 */

#include "CapEngine.h"

namespace CapEngine
{
/*
  \brief Class representing a circle
*/
class Circle
{
public:
  Circle(double x, double y, double r);
  double getX() const;
  void setX(double xIn);
  double getY() const;
  void setY(double yIn);
  double getRadius() const;
  void setRadius(double rIn);
  bool collides(const Circle *circle);

private:
  // hidden functions
  Circle(const Circle &) {}
  Circle &operator=(const Circle &) { return *this; }

private:
  double x, y;
  double radius;
};
} // namespace CapEngine
#endif
