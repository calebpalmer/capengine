#include "quaternion.h"

#include "CapEngineException.h"
#include "capcommon.h"
#include "trigonometry.h"

#include <cmath>

namespace CapEngine
{

//! Constructor
/**
 \param v - The axis of rotation
 \param angle - The angle of rotation
 \return
*/
Quaternion::Quaternion(const Vector &v, double angle)
{
  m_w = cos(DEGTORAD(angle / 2.0));

  // make sure v is a unit vector
  Vector normalized = v.normalize();

  double multiplier = sin(DEGTORAD(angle / 2.0));
  m_x = multiplier * normalized.getX();
  m_y = multiplier * normalized.getY();
  m_z = multiplier * normalized.getZ();
}

//! Constructor
/**
 \param w - The w value
 \param x - The x value
 \param y - The y value
 \param z - The z value
*/
Quaternion::Quaternion(double w, double x, double y, double z)
    : m_w(w), m_x(x), m_y(y), m_z(z)
{
  // TODO want to check if the quaternion is valid, but below is not working
  // 	double magnitude = this->magnitude();
  // 	if(!doubles_equal(1.0, magnitude)){
  // 		BOOST_THROW_EXCEPTION(CapEngineException("Not a valid
  // quaternion"));
  // 	}
}

//! Getter for w
double Quaternion::getW() const { return m_w; }

//! Getter for x
double Quaternion::getX() const { return m_x; }

//! Getter for y
double Quaternion::getY() const { return m_y; }

//! Getter for z
double Quaternion::getZ() const { return m_z; }

//! Returns the quaternion negated
/**
 \return - The negated quaternion
*/
Quaternion Quaternion::negate() const
{
  return Quaternion(-m_w, -m_x, -m_y, -m_z);
}

//! Returns the conjugate of the quaternion
/**
         \return - The conjugate of the quaternion
 */
Quaternion Quaternion::conjugate() const
{
  return Quaternion(m_w, -m_x, -m_y, -m_z);
}

//! Get the inverse of the quaternion
/**
 \return - The inverse
*/
Quaternion Quaternion::inverse() const
{
  // since the vector representing the axis is always a unit vector
  // (we ensured this during construction) the inverse == conjugate
  return this->conjugate();
}

//! operato==
/**
 \param q1 - the lhs
 \param q2 - the rhs
 \return true if they are equal,false otherwise
*/
bool Quaternion::operator==(const Quaternion &q2) const
{
  return this->getW() == q2.getW() && this->getX() == q2.getX() &&
         this->getY() == q2.getY() && this->getZ() == q2.getZ();
}

//! Performs quaternion multiplication
/**
 \param q2 - The quaternion to multiply by
 \return - The resulting quaternion
*/
Quaternion Quaternion::operator*(const Quaternion &q2) const
{
  const Quaternion &q1 = *this;
  /**
           q1 * q2 =
               w' = w1 * w2 - v1 dot v2
                           v' = (w1 * v2) + (w2 * v1) + v2 cross v1
   */

  Vector v1(q1.m_x, q1.m_y, q1.m_z);
  Vector v2(q2.m_x, q2.m_y, q2.m_z);

  double w3 = (q1.m_w * q2.m_w) - dotProduct(v1, v2);
  Vector v3 = (v2 * q1.m_w) + (v1 * q2.m_w) + crossProduct(v2, v1);

  return Quaternion(w3, v3.x, v3.y, v3.z);
}

//! Computes the magnitude of the quaternion.
/**

 Should always be 1.

 \return - The magnitude of the quaternion
*/
double Quaternion::magnitude() const
{
  return std::sqrt(std::pow(m_w, 2) +
                   std::pow(Vector(m_x, m_y, m_x).magnitude(), 2));
}

} // namespace CapEngine
