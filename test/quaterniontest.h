#ifndef CAPENGINE_QUATERNIONTEST_H
#define CAPENGINE_QUATERNIONTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../quaternion.h"
#include "../vector.h"

namespace CapEngine {

	class QuaternionTest : public CppUnit::TestFixture {
	public:
		void testNormalizes();
		void testNegate();
		void testConjugate();
		void testInverse();
		
		CPPUNIT_TEST_SUITE(QuaternionTest);
		CPPUNIT_TEST(testNegate);
		CPPUNIT_TEST(testNormalizes);
		CPPUNIT_TEST(testConjugate);
		CPPUNIT_TEST(testInverse);
		CPPUNIT_TEST_SUITE_END();
	};

	//! tests negating a quaternion
	void QuaternionTest::testNegate(){
		Quaternion q(1.0, 2.0, 3.0, 4.0);
		Quaternion qNeg = q.negate();
		CPPUNIT_ASSERT(qNeg.getW() == -1.0 && qNeg.getX() == -2.0 &&
									 qNeg.getY() == -3.0 && qNeg.getZ() == -4.0);

	}
	
	//! test that vector passed in is normalized
	void QuaternionTest::testNormalizes(){
		Vector v(2.0, 5.0, 3.0);
		Vector normalized = v.normalize();
		double angle = 25;

		Quaternion q1(v, angle);
		Quaternion q2(normalized, angle);

		CPPUNIT_ASSERT_EQUAL(q1.getW(), q2.getW());
		CPPUNIT_ASSERT_EQUAL(q1.getX(), q2.getX());
		CPPUNIT_ASSERT_EQUAL(q1.getY(), q2.getY());
		CPPUNIT_ASSERT_EQUAL(q1.getZ(), q2.getZ());
	}

	void QuaternionTest::testConjugate(){
		Quaternion q(1.0, 2.0, 3.0, 4.0);
		Quaternion q2 = q.conjugate();
		CPPUNIT_ASSERT(q2.getW() == 1.0 && q2.getX() == -2.0 &&
									 q2.getY() == -3.0 && q2.getZ() == -4.0);
		
	}

	void QuaternionTest::testInverse(){
		Quaternion q(1.0, 2.0, 3.0, 4.0);
		Quaternion q2 = q.conjugate();
		CPPUNIT_ASSERT(q2.getW() == 1.0 && q2.getX() == -2.0 &&
									 q2.getY() == -3.0 && q2.getZ() == -4.0);
		
	}


}

#endif // CAPENGINE_QUATERNIONTEST_H
