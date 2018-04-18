#include "quaternion.h"

#include <cmath>

#include "trigonometry.h"

namespace CapEngine {

//! Constructor
/** 
 \param v - The axis of rotation
 \param angle - The angle of rotation
 \return 
*/
Quaternion::Quaternion(const Vector &v, double angle){
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
Quaternion::Quaternion(double w, double x, double y, double z) :
	m_w(w), m_x(x), m_y(y), m_z(z)
{
}

//! Getter for w
double Quaternion::getW() const{
	return m_w;
}

//! Getter for x
double Quaternion::getX() const{
	return m_x;
}

//! Getter for y
double Quaternion::getY() const{
	return m_y;
}

//! Getter for z
double Quaternion::getZ() const{
	return m_z;
}

//! Returns the quaternion negated
/** 
 \return - The negated quaternion
*/
Quaternion Quaternion::negate() const{
	return Quaternion(-m_w, -m_x, -m_y, -m_z);
}

//! Returns the conjugate of the quaternion
/**
	 \return - The conjugate of the quaternion
 */
Quaternion Quaternion::conjugate() const{
	return Quaternion(m_w, -m_x, -m_y, -m_z);
}

//! operato==
/** 
 \param q1 - the lhs
 \param q2 - the rhs
 \return true if they are equal,false otherwise
*/
bool Quaternion::operator==(const Quaternion& q2){
	return
		this->getW() == q2.getW() &&
		this->getX() == q2.getX() &&
		this->getY() == q2.getY() &&
		this->getZ() == q2.getZ();
}

}
