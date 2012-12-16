#ifndef CIRCLE_H
#define CIRCLE_H

/*
  \brief Header file for circle class
  \author Caleb Palmer
 */

#include "CapEngine.h"

namespace CapEngine{
/*
  \brief Class representing a circle
*/
class Circle{
 public:
  Circle(real x, real y, real r);
  real getX() const;
  void setX(real xIn);
  real getY() const;
  void setY(real yIn);
  real getRadius() const;
  void setRadius(real rIn);
  bool collides(const Circle* circle);
  
 private:
  // hidden functions
  Circle(const Circle&){}
  Circle& operator=(const Circle&){return *this;}

 private:
  real x, y;
  real radius;
  
};
}
#endif
