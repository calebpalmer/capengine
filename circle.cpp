#include "circle.h"
#include "CapEngineException.h"
#include "geometry.h"
#include "Vector2d.h"

using namespace CapEngine;

/*
  \brief Circle constructor
  \param x
  \li the x coordinate of the center point of the circle
  \param y
  \li the y coordinate of the center point of the circle
  \param r
  \li The radius of the circle
 */
Circle::Circle(real x, real y, real r) : x(x), y(x), radius(r) {};

real Circle::getX() const{
  return x;
}

void Circle::setX(real xIn){
  x = xIn;
}

real Circle::getY() const{
  return y;
}

void Circle::setY(real yIn){
  y = yIn;
}

real Circle::getRadius() const{
  return radius;
}

void Circle::setRadius(real radiusIn){
  radius = radiusIn;
}

/*
  \brief detects whether this circle collides with another
  \param circle
  \li the Circle to detect whether this circle collides with
  \return
  \li true if they collide, false otherwise
*/
bool Circle::collides(const Circle* circle){
  CAP_THROW_NULL(circle, "circle passed in was null");

  Vector2d vector1(this->x, this->y);
  Vector2d vector2(circle->getX(), circle->getY());
  real centerDistance = getDistance(&vector1, &vector2);
  if(centerDistance < 0)
    centerDistance *= -1.0;
  return centerDistance <= (this->radius + circle->getRadius());
}
