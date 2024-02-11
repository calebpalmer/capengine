//@brief - implementation file for the Vector class
//@author - Caleb Palmer

#include "vector.h"

#include <boost/exception/all.hpp>
#include <cmath>
#include <cstddef>
#include <sstream>

#include "CapEngineException.h"
#include "capcommon.h"
#include "captypes.h"
#include "trigonometry.h"

namespace CapEngine
{

using namespace std;

// default constructor for a Vector.  Sets everything to zeros.
Vector::Vector() : x(0.0), y(0.0), z(0.0), d(1.0) {}

/* constructor for Vector
   \param x_in the x component of the vector
   \param y_in the y component of the vector
   \param slopeX_in - the slope for the x component of the vector
   \param slopeY_in - the slope for the y component of the vector
*/
Vector::Vector(double x_in, double y_in, double z_in, double d_in)
	: x(x_in), y(y_in), z(z_in), d(d_in)
{
}

/* constructor for Vector
   \param x_in the x component of the vector
   \param y_in the y component of the vector
   \param slopeX_in - the slope for the x component of the vector
   \param slopeY_in - the slope for the y component of the vector
*/
Vector::Vector(int x_in, int y_in, int z_in, int d_in)
	: x(static_cast<double>(x_in)),
	  y(static_cast<double>(y_in)),
	  z(static_cast<double>(z_in)),
	  d(static_cast<double>(d_in))
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
  \return
																																\li the x
  component of the vector
*/
double Vector::getX() const { return x; }

/** returns y component of vector
  \return
																																\li the y
  component of the vector
*/
double Vector::getY() const { return y; }

/** returns z component of vector
  \return
																																\li the z
  component of the vector
*/
double Vector::getZ() const { return z; }

/** returns d component of vector
  \return
																																\li the d
  component of the vector
*/
double Vector::getD() const { return d; }

/** set the x componnet
  \param
																																 \li the x
  component
*/
void Vector::setX(double xIn) { x = xIn; }

/** set the y componnet
  \param
																																\li the y
  component
*/
void Vector::setY(double yIn) { y = yIn; }

/** set the y componnet
  \param
																																\li the y
  component
*/
void Vector::setZ(double zIn) { z = zIn; }

/** set the d componnet
  \param
																																\li the d
  component
*/
void Vector::setD(double dIn) { d = dIn; }

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
Vector Vector::operator*(double scalar) const
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
double distance(const Vector &vec1, const Vector &vec2)
{
	// sqrt (x*x + y*y
	double xdelta = vec1.getX() - vec2.getX();
	double ydelta = vec1.getY() - vec2.getY();
	double zdelta = vec1.getZ() - vec2.getZ();
	return sqrt(xdelta * xdelta + ydelta * ydelta + zdelta * zdelta);
}

/*
  \fn multiply the members of the vector by given factor
  \param factor
  \li the amount to scale the vector components
*/
void Vector::scale(double factor)
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
double Vector::magnitude() const { return distance(*this, Vector(0, 0, 0)); }

/*
  \fn return a normalized version of this vector
  \return
  \li ptr to a normalized Vector
*/
Vector Vector::normalize() const
{
	double mag = this->magnitude();
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
double dotProduct(const Vector &vec1, const Vector &vec2)
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
	double newx = vec1.getY() * vec2.getZ() + vec1.getZ() * vec2.getY();
	double newy = vec1.getZ() * vec2.getX() + vec1.getX() * vec2.getZ();
	double newz = vec1.getX() * vec2.getY() + vec1.getY() * vec2.getX();

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
	double dotProd = CapEngine::dotProduct(vec1, vec2);
	double magVec = vec2.magnitude();
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
double angle(const Vector &vec1, const Vector &vec2)
{
	// acos (vec.x - x / distance)
	double distance = CapEngine::distance(vec1, vec2);
	double xDelta = vec1.getX() - vec2.getX();
	double radAngle = acos(distance / xDelta);
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

double Vector::slope() const
{
	if (z != 0.0) {
		BOOST_THROW_EXCEPTION(
			CapEngineException("slope only supports 2D vectors"));
	}

	return y / x;
}

//! return the angle in radians of the vector in 2d.
/*
  \return
   \li The angle.
 */
double Vector::angle2d() const { return atan2(this->y, this->x); }

Vector Vector::absolute() const
{
	return Vector{std::abs(x), std::abs(y), std::abs(z), std::abs(d)};
}

}  // namespace CapEngine
