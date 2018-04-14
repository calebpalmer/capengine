#ifndef CAPENGINE_QUATERNIAN_H
#define CAPENGINE_QUATERNIAN_H

#include "vector.h"

namespace CapEngine {
	
	class Quaternian {
	public:
		Quaternian(const Vector &v, double angle);
	private:
		double m_w = 0;
		double m_x = 0;
		double m_y = 0;
		double m_z = 0;
	};
	
}

#endif // CAPENGINE_QUATERNIAN_H
