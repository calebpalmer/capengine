#ifndef CAPENGINE_QUATERNIONTEST_H
#define CAPENGINE_QUATERNIONTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../quaternion.h"

namespace CapEngine {

	namespace {

		class TestQuaternion : public Quaternion {
		public:
		TestQuaternion(double w, double x, double y, double z) :
			Quaternion(w, x, y, z) {}
			
			TestQuaternion(const Quaternion& q)
				: Quaternion(q) {}
			
			
			using Quaternion::m_w;
			using Quaternion::m_x;
			using Quaternion::m_y;
			using Quaternion::m_z;

		};
	}

	class QuaternionTest : public CppUnit::TestFixture {
	public:
		void testNegate();
		
		CPPUNIT_TEST_SUITE(QuaternionTest);
		CPPUNIT_TEST(testNegate);
		CPPUNIT_TEST_SUITE_END();
	};

	//! tests negating a quaternion
void QuaternionTest::testNegate(){
	TestQuaternion q(1.0, 2.0, 3.0, 4.0);
	TestQuaternion qNeg = q.negate();
	CPPUNIT_ASSERT(qNeg.m_w == -1.0 && qNeg.m_x == -2.0 &&
								 qNeg.m_y == -3.0 && qNeg.m_z == -4.0);
}

	
}

#endif // CAPENGINE_QUATERNIONTEST_H
