#include "geometry.h"
#include "CapEngineException.h"

#include <memory>
#include <math.h>

using namespace std; 

namespace CapEngine{

  
  /*
    \fn This function determines whether two vectors every intersect
    \param vector1
    \li 
    \param vector2
    \li 
    \return
    \li true if they do intersect at some point, false if they do not
  */
  bool doVectorsIntersect2d(const Vector2d* vector1, const Vector2d* vector2){
    CAP_THROW_NULL(vector1, "vector is null");
    CAP_THROW_NULL(vector2, "vector is null");

    // if slopes are different then they do
    real slope1 = vector1->getYSlope() / vector1->getXSlope();
    real slope2 = vector2->getYSlope() / vector2->getXSlope();
    if(slope1 != slope2){
      return true;
    }
    // if slopes are the same and same y-intercept then they do
    // false otherwise
    return (vector1->getYIntercept() == vector2->getYIntercept());

  }

  /*
    \fn This function returns the point at which two vectors intersect
    \param vector1
    \li
    \param vector2
    \li
    \return
    \li A pointer to a vector with the location of intersection, NULL if they do not intersect
   */
  Vector2d* vectorIntersectionPoint2d(const Vector2d* vector1
				      , const Vector2d* vector2){

    CAP_THROW_NULL(vector1, "vector is null");
    CAP_THROW_NULL(vector2, "vector is null");
    // y = m1(x + x1) + y1 = m2(x + x2) + y2
    // x = (m1x1 - m2x2) + y2 - y1)/(m1 - m2)
    // y = -mx1 + y1
    real slope1 = vector1->getYSlope() / vector1->getXSlope();
    real slope2 = vector2->getYSlope() / vector2->getXSlope();
    real x = ((slope1 * vector1->getX() - slope2 * vector2->getX()) + vector2->getY() - vector1->getY()) / (slope1 - slope2);
    real y = (slope1 * (-1)) * vector1->getX() + vector1->getY();
    std::auto_ptr<Vector2d> position(new Vector2d(x, y));
    return position.release();
  }


  /*
    \fn This function returns the distance between two vectors
    \param vector1
    \li
    \param vector2
    \li
    \return
    \li A pointer to a vector with the location of intersection, NULL if they do not intersect
   */
  real getDistance(const Vector2d* vector1, const Vector2d* vector2){
    CAP_THROW_NULL(vector1, "vector is null");
    CAP_THROW_NULL(vector2, "vector is null");
    
    // pythagoean theorem
    // c^2 = a^2 + b^2
    // c = sqrt((x2 - x1)^2 + (y2 - y1)^2)
    real xDelta = vector2->getX() - vector1->getX();
    real yDelta = vector2->getY() - vector1->getY();
    return sqrt((xDelta*xDelta) + (yDelta*yDelta));
    
  }


  /*
    \fn This function returns the distance between two vectors
    ((x1 + x2) / 2, (y1 + y2) / 2)
    \param vector1
    \li
    \param vector2
    \li
    \return
    \li A pointer to a vector with the location of intersection, NULL if they do not intersect
  */
  Vector2d* midPoint(const Vector2d* vector1, const Vector2d* vector2){
    CAP_THROW_NULL(vector1, "vector is null");
    CAP_THROW_NULL(vector2, "vector is null");
    
    real x = (vector1->getX() + vector2->getX()) / 2.0;
    real y = (vector1->getY() + vector2->getY()) / 2.0;

    auto_ptr<Vector2d> newVector(new Vector2d(x, y));

    return newVector.release();
  }

  
}


