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

	double multiplier = sin(DEGTORAD(angle / 2.0));
	m_x = multiplier * v.getX();
	m_y = multiplier * v.getY();
	m_z = multiplier * v.getZ();
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



//! Returns the quaternion negated
/** 
 \return - The negated quaternion
*/
Quaternion Quaternion::negate() const{
	return Quaternion(-m_w, -m_x, -m_y, -m_z);
}

}
