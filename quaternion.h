#ifndef CAPENGINE_QUATERNIAN_H
#define CAPENGINE_QUATERNIAN_H

#include "vector.h"

namespace CapEngine {
	
	class Quaternion {
	public:
		Quaternion(const Vector &v, double angle);

		Quaternion negate() const;
	protected:
		Quaternion(double w, double x, double y, double z);
		
		double m_w = 0;
		double m_x = 0;
		double m_y = 0;
		double m_z = 0;
	};
	
}

#endif // CAPENGINE_QUATERNIAN_H
