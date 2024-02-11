#include "circle.h"
#include "CapEngineException.h"
#include "Vector2d.h"
#include "geometry.h"

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
Circle::Circle(double x, double y, double r) : x(x), y(x), radius(r){};

double Circle::getX() const { return x; }

void Circle::setX(double xIn) { x = xIn; }

double Circle::getY() const { return y; }

void Circle::setY(double yIn) { y = yIn; }

double Circle::getRadius() const { return radius; }

void Circle::setRadius(double radiusIn) { radius = radiusIn; }

/*
  \brief detects whether this circle collides with another
  \param circle
  \li the Circle to detect whether this circle collides with
  \return
  \li true if they collide, false otherwise
*/
bool Circle::collides(const Circle *circle)
{
  CAP_THROW_NULL(circle, "circle passed in was null");

  Vector2d vector1(this->x, this->y);
  Vector2d vector2(circle->getX(), circle->getY());
  double centerDistance = getDistance(&vector1, &vector2);
  if (centerDistance < 0)
	centerDistance *= -1.0;
  return centerDistance <= (this->radius + circle->getRadius());
}
