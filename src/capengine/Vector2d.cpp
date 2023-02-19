//@brief - implementation file for the Vector2d class
//@author - Caleb Palmer

#include "Vector2d.h"
#include "trigonometry.h"
#include <cmath>
#include <cstddef>

using namespace CapEngine;
using namespace std;

// default constructor for a Vector2d.  Sets everything to zeros.
Vector2d::Vector2d() : x(0), y(0) {}

/* constructor for Vector2d
   \param x_in the x component of the vector
   \param y_in the y component of the vector
   \param slopeX_in - the slope for the x component of the vector
   \param slopeY_in - the slope for the y component of the vector
*/
Vector2d::Vector2d(real x_in, real y_in) : x(x_in), y(y_in) {}

// copy constructor
Vector2d::Vector2d(const Vector2d &vec)
{
  this->x = vec.x;
  this->y = vec.y;
}

// constructor taking a PolarVector as input
Vector2d::Vector2d(const PolarVector &pVecIn)
{
  x = pVecIn.mag * cos(DEGTORAD(pVecIn.deg));
  y = pVecIn.mag * sin(DEGTORAD(pVecIn.deg));
}

Vector2d::~Vector2d() {}

/** returns x component of vector
    \returns the x component of the vector
*/
real Vector2d::getX() const { return x; }

/** returns y component of vector
    \returns the y component of the vector
*/
real Vector2d::getY() const { return y; }

/** set the x componnet
    \param the x component
*/
void Vector2d::setX(real xIn) { x = xIn; }

/** set the y componnet
    \param the y component
*/
void Vector2d::setY(real yIn) { y = yIn; }

/*
  \fn operator+ definition
  \param vec1
  \param vec2
  \return Vector2d
 */
Vector2d Vector2d::operator+(const Vector2d &vec2)
{
  return Vector2d(x + vec2.getX(), y + vec2.getY());
}

/*
  \fn operator- definition
  \param vec1
  \param vec2
  \return Vector2d
 */
Vector2d Vector2d::operator-(const Vector2d &vec2)
{
  return Vector2d(x - vec2.getX(), y - vec2.getY());
}

/*
  \fn calculate the distance from the given Vector
  \param vec
  \li the vector to calculate the distance from
  \return
  \li the distance of this from vec.
 */
real Vector2d::distance(const Vector2d &vec) const
{
  // sqrt (x*x + y*y
  real xdelta = x - vec.getX();
  real ydelta = y - vec.getY();
  return sqrt(xdelta * xdelta + ydelta * ydelta);
}

/*
  \fn multiply the members of the vector by given factor
  \param factor
  \li the amount to scale the vector components
 */
void Vector2d::scale(real factor)
{
  x = x * factor;
  y = y * factor;
}

/*
  \fn calculate the magnitude of the vector
  \return
  \li the magnitude of the vector
 */
real Vector2d::magnitude() const { return distance(Vector2d(0, 0)); }

/*
  \fn return a normalized version of this vector
  \return
  \li ptr to a normalized Vector2d
 */
Vector2d *Vector2d::normalize() const
{
  real mag = this->magnitude();
  unique_ptr<Vector2d> normalVec(new Vector2d(x / mag, y / mag));
  return normalVec.release();
}

/*
  \fn calculate the dot product of this and passed vector
  \param vec
  \return
  \li the dot product
 */
real Vector2d::dotProduct(const Vector2d &vec) const
{
  return (x * vec.getX() + y * vec.getY());
}

/*
  \fn calculate the crossProduct of this vector and passed vector
  \param vec
  \return
  \li the cross product of the two vectors
 */
Vector2d *Vector2d::crossProduct(const Vector2d &vec) const
{
  // makes no sense to calculate crossProduct for 2d vectors
  throw CapEngineException("Tried to calculate cross product of 2d vector");
}

/*
  \fn calculate the angle between this vector and the given vector
  \param vec
  \return
  \li the angle
 */
real Vector2d::angle(const Vector2d &vec) const
{
  // acos (vec.x - x / distance)
  real distance = this->distance(vec);
  real xDelta = x - vec.getX();
  real radAngle = acos(distance / xDelta);
  return RADTODEG(radAngle);
}

//! Not implemented for 2d vectors.  Throws exception.
/*!
 */
Vector2d *Vector2d::surfaceNormal(const Vector2d &vec) const
{
  throw CapEngineException(
      "Tried to calculate the surface normal of 2d vectors");
}

//! return a PolarVector version of current vector
/*!
  \return
  \li pointer to a PolarVector.  Ownership is passed.
 */
PolarVector *Vector2d::toPolarVector() const { return nullptr; }
