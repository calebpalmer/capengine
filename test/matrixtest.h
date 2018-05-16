#ifndef MATIXTEST_H
#define MATRIXTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../matrix.h"
#include "../vector.h"

namespace CapEngine {

  class MatrixTest : public CppUnit::TestFixture {
  public:
		void testIdentiyMatrixTimesVector();
		//void testMatrixTranslateRight();
		//void testMatrixTranslateDown();
		void testMatrixMultiplyIdentityTranslation();

    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(testIdentiyMatrixTimesVector);
    //CPPUNIT_TEST(testMatrixTranslateDown);
    //CPPUNIT_TEST(testMatrixTranslateRight);
		CPPUNIT_TEST(testMatrixMultiplyIdentityTranslation);
    CPPUNIT_TEST_SUITE_END();

	};

	// TODO rewrite to use translation matrix
	void MatrixTest::testIdentiyMatrixTimesVector(){
		using namespace CapEngine;
      
		Matrix matrix = Matrix::createIdentityMatrix();
		Vector vector(1.0, 1.0, 1.0, 1.0);
		Vector newVector = matrix * vector;
		CPPUNIT_ASSERT(newVector.x == 1.0
									 && newVector.y == 1.0
									 && newVector.z == 1.0
									 && newVector.d == 1.0);
	}

	// rewrite to use translation matrix
	/* void MatrixTest::testMatrixTranslateRight(){ */

	/* 	using namespace CapEngine; */

      
	/* 	Matrix matrix = Matrix::createIdentityMatrix(); */
	/* 	double xTranslateValue = -20.0; */
	/* 	matrix.getRowVectorRef(0).setD(xTranslateValue); */
      
	/* 	Vector vector(1.0, 1.0, 1.0, 1.0); */
	/* 	Vector newVector = matrix * vector; */
	/* 	CPPUNIT_ASSERT(newVector.x == -19.0 */
	/* 								 && newVector.y == 1.0 */
	/* 								 && newVector.z == 1.0 */
	/* 								 && newVector.d == 1.0); */

	/* } */

	// TODO rewrite to use translation matrix
	/* void MatrixTest::testMatrixTranslateDown(){ */

	/* 	using namespace CapEngine; */
      
	/* 	Matrix matrix = Matrix::createIdentityMatrix(); */
	/* 	double yTranslateValue = 20.0; */
	/* 	matrix.getRowVectorRef(1).setD(yTranslateValue); */
      
	/* 	Vector vector(1.0, 1.0, 1.0, 1.0); */
	/* 	Vector newVector = matrix * vector; */
	/* 	CPPUNIT_ASSERT(newVector.x == 1.0 */
	/* 								 && newVector.y == 21.0 */
	/* 								 && newVector.z == 1.0 */
	/* 								 && newVector.d == 1.0); */

	/* } */

	//! tests multiplying an identity matrix by a translation matrix
	void MatrixTest::testMatrixMultiplyIdentityTranslation(){

		Matrix m1 = Matrix::createTranslationMatrix(2.0, 1.0, -0.5);
		Matrix m2 = Matrix::createIdentityMatrix();
		Matrix result = m1 * m2;

		Matrix expectedResult({1.0, 0.0, 0.0, 0.0},
													{0.0, 1.0, 0.0, 0.0},
													{0.0, 0.0, 1.0, 0.0},
													{2.0, 1.0, -0.5, 1.0});

		CPPUNIT_ASSERT(expectedResult == result);
	}

}

#endif //VECTORTEST
