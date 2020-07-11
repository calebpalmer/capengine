//@brief - implementation file for the Vector class
//@author - Caleb Palmer

#include "vector.h"
#include "CapEngineException.h"
#include "capcommon.h"
#include "trigonometry.h"

#include <boost/exception/all.hpp>
#include <cmath>
#include <cstddef>
#include <sstream>

namespace CapEngine
{

using namespace std;

// default constructor for a Vector.  Sets everything to zeros.
Vector::Vector() : x(0.0), y(0.0), z(0.0), d(0.0) {}

/* constructor for Vector
   \param x_in the x component of the vector
   \param y_in the y component of the vector
   \param slopeX_in - the slope for the x component of the vector
   \param slopeY_in - the slope for the y component of the vector
*/
Vector::Vector(real x_in, real y_in, real z_in, real d_in)
    : x(x_in), y(y_in), z(z_in), d(d_in)
{
}

// copy constructor
Vector::Vector(const Vector &vec)
{
  this->x = vec.x;
  this->y = vec.y;
  this->z = vec.z;
  this->d = vec.d;
}

// constructor taking a PolarVector as input, creates a vector on the xy plane
Vector::Vector(const PolarVector &pVecIn)
{
  x = pVecIn.mag * cos(DEGTORAD(pVecIn.deg));
  y = pVecIn.mag * sin(DEGTORAD(pVecIn.deg));
  z = 0.0;
  d = 0.0;
}

Vector::~Vector() {}

/** returns x component of vector
    \returns the x component of the vector
*/
real Vector::getX() const { return x; }

/** returns y component of vector
    \returns the y component of the vector
*/
real Vector::getY() const { return y; }

/** returns z component of vector
    \returns the z component of the vector
*/
real Vector::getZ() const { return z; }

/** returns d component of vector
    \returns the d component of the vector
*/
real Vector::getD() const { return d; }

/** set the x componnet
    \param the x component
*/
void Vector::setX(real xIn) { x = xIn; }

/** set the y componnet
    \param the y component
*/
void Vector::setY(real yIn) { y = yIn; }

/** set the y componnet
    \param the y component
*/
void Vector::setZ(real zIn) { z = zIn; }

/** set the d componnet
    \param the d component
*/
void Vector::setD(real dIn) { d = dIn; }

/*
  \fn operator+ definition
  \param vec1
  \param vec2
  \return Vector
*/
Vector Vector::operator+(const Vector &vec2) const
{
  return Vector(x + vec2.getX(), y + vec2.getY(), z + vec2.getZ(),
                d + vec2.getD());
}

Vector &Vector::operator+=(const Vector &vec)
{
  this->x += vec.x;
  this->y += vec.y;
  this->z += vec.z;
  this->d += vec.d;

  return *this;
}

/*
  \fn operator- definition
  \param vec1
  \param vec2
  \return Vector
*/
Vector Vector::operator-(const Vector &vec2) const
{
  return Vector(x - vec2.getX(), y - vec2.getY(), z - vec2.getZ());
}

//! multipllies vector by a scalar
/**
 \param scalar - The scalar to multiply by.
 \return - The resulting Vector
*/
Vector Vector::operator*(real scalar) const
{
  // TODO does not scale d.  Is this right?
  return Vector(x * scalar, y * scalar, z * scalar, d);
}

// TODO: fix this so that it returns a proper value
/*
  \fn operator= definition
  \param vec1q
  \param vec2
  \return Vector
*/
Vector &Vector::operator=(const Vector &vec)
{
  Vector newVector;
  x = vec.x;
  y = vec.y;
  z = vec.z;
  d = vec.d;
  return *this;
}

bool Vector::operator==(const Vector &vec) const
{

  return doubles_equal(this->x, vec.x) && doubles_equal(this->y, vec.y) &&
         doubles_equal(this->z, vec.z) && doubles_equal(this->d, vec.d);
}

//! operator!=
bool Vector::operator!=(const Vector &other) const { return !(*this == other); }

// TODO
/*
  \fn calculate the distance from the given Vector
  \param vec
  \li the vector to calculate the distance from
  \return
  \li the distance of this from vec.
*/
real distance(const Vector &vec1, const Vector &vec2)
{
  // sqrt (x*x + y*y
  real xdelta = vec1.getX() - vec2.getX();
  real ydelta = vec1.getY() - vec2.getY();
  real zdelta = vec1.getZ() - vec2.getZ();
  return sqrt(xdelta * xdelta + ydelta * ydelta + zdelta * zdelta);
}

/*
  \fn multiply the members of the vector by given factor
  \param factor
  \li the amount to scale the vector components
*/
void Vector::scale(real factor)
{
  x = x * factor;
  y = y * factor;
  z = z * factor;
}

/*
  \fn calculate the magnitude of the vector
  \return
  \li the magnitude of the vector
*/
real Vector::magnitude() const { return distance(*this, Vector(0, 0, 0)); }

/*
  \fn return a normalized version of this vector
  \return
  \li ptr to a normalized Vector
*/
Vector Vector::normalize() const
{
  real mag = this->magnitude();
  return Vector(x / mag, y / mag, z / mag);
}

/*
  \fn calculate the dot product of this and passed vector
  Generally tells how similar two vectors are
  if > 0 -> theta between 0 and 90
  if == 0 -> theta is 90
  if < 0 -> theta between 90 and 180
  \param vec
  \return
  \li the dot product
*/
real dotProduct(const Vector &vec1, const Vector &vec2)
{
  return ((vec1.getX() * vec2.getX()) + (vec1.getY() * vec2.getY()) +
          (vec1.getZ() * vec2.getZ()) + (vec1.getD() * vec2.getD()));
}

/*
  \fn calculate the crossProduct of this vector and passed vector
  \param vec
  \return
  \li the cross product of the two vectors
*/
Vector crossProduct(const Vector &vec1, const Vector &vec2)
{
  real newx = vec1.getY() * vec2.getZ() + vec1.getZ() * vec2.getY();
  real newy = vec1.getZ() * vec2.getX() + vec1.getX() * vec2.getZ();
  real newz = vec1.getX() * vec2.getY() + vec1.getY() * vec2.getX();

  return Vector(newx, newy, newz);
}

/*
  \fn calculate the projected vector of this onto vec
  \param vec the vector to be projected onto
  \return
  \li the project vector of this onto vec
*/
Vector projectedVector(const Vector &vec1, const Vector &vec2)
{
  real dotProd = CapEngine::dotProduct(vec1, vec2);
  real magVec = vec2.magnitude();
  Vector v(vec2);
  v.scale(dotProd / (magVec * magVec));
  return v;
}

/*
  \fn calculate the perpendicular of the projected vector of this onto vec
  \param vec the vector to be projected onto
  \return
  \li the project vector of this onto vec
*/
Vector projectedPerpendicularVector(const Vector &vec1, const Vector &vec2)
{
  Vector v = CapEngine::projectedVector(vec1, vec2);
  return Vector(vec2 - v);
}

/*
  \fn calculate the angle between this vector and the given vector along xy axes
  \param vec
  \return
  \li the angle
*/
real angle(const Vector &vec1, const Vector &vec2)
{
  // acos (vec.x - x / distance)
  real distance = CapEngine::distance(vec1, vec2);
  real xDelta = vec1.getX() - vec2.getX();
  real radAngle = acos(distance / xDelta);
  return RADTODEG(radAngle);
}

//! Not implemented for 2d vectors.  Throws exception.
/*!
 */
Vector surfaceNormal(const Vector &vec1, const Vector &vec2)
{
  Vector v = CapEngine::crossProduct(vec1, vec2);
  return v.normalize();
}

//! return a PolarVector version of current vector
/*!
  \return
  \li pointer to a PolarVector.  Ownership is passed.
*/
PolarVector *Vector::toPolarVector() const { return nullptr; }

/**
   return a textual representation of a vector
*/
std::string Vector::toString() const
{
  ostringstream desc;
  desc << "  " << x << "  " << y << "  " << z << "  " << d;
  return desc.str();
}

std::ostream &operator<<(std::ostream &stream, const CapEngine::Vector &vector)
{
  stream << "Vector(" << vector.x << ", " << vector.y << ", " << vector.z
         << ", " << vector.d << ")";
  return stream;
}

real Vector::slope() const
{
  if (z != 0.0) {
    BOOST_THROW_EXCEPTION(CapEngineException("slope only supports 2D vectors"));
  }

  return y / x;
}

Vector Vector::absolute() const
{
  return {std::abs(x), std::abs(y), std::abs(z), std::abs(d)};
}

} // namespace CapEngine
