#ifndef MATIXTEST_H
#define MATRIXTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../matrix.h"
#include "../vector.h"

  class MatrixTest : public CppUnit::TestFixture {
  public:
    void setUp() {}

    void tearDown() {}

    void testIdentiyMatrixTimesVector(){
      using namespace CapEngine;
      
      Matrix matrix = Matrix::createIdentityMatrix();
      Vector vector(1.0, 1.0, 1.0, 1.0);
      Vector newVector = matrix * vector;
      CPPUNIT_ASSERT(newVector.x == 1.0
		     && newVector.y == 1.0
		     && newVector.z == 1.0
		     && newVector.d == 1.0);
    }

    void testMatrixTranslateRight(){

      using namespace CapEngine;

      
      Matrix matrix = Matrix::createIdentityMatrix();
      double xTranslateValue = -20.0;
      matrix.getRowVectorRef(0).setD(xTranslateValue);
      
      Vector vector(1.0, 1.0, 1.0, 1.0);
      Vector newVector = matrix * vector;
      CPPUNIT_ASSERT(newVector.x == -19.0
		     && newVector.y == 1.0
		     && newVector.z == 1.0
		     && newVector.d == 1.0);

    }

    void testMatrixTranslateDown(){

      using namespace CapEngine;

      
      Matrix matrix = Matrix::createIdentityMatrix();
      double yTranslateValue = 20.0;
      matrix.getRowVectorRef(1).setD(yTranslateValue);
      
      Vector vector(1.0, 1.0, 1.0, 1.0);
      Vector newVector = matrix * vector;
      CPPUNIT_ASSERT(newVector.x == 1.0
		     && newVector.y == 21.0
		     && newVector.z == 1.0
		     && newVector.d == 1.0);

    }


    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(testIdentiyMatrixTimesVector);
    CPPUNIT_TEST(testMatrixTranslateDown);
    CPPUNIT_TEST(testMatrixTranslateRight);
    CPPUNIT_TEST_SUITE_END();

  };

#endif //VECTORTEST
