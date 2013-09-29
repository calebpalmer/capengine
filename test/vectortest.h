#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../vector.h"

class VectorTest : public CppUnit::TestFixture {
  void setup() {}

  void teardown() {}

  void testVectorAdditionLessThanOne(){
    CapEngine::Vector v1(1.0, 1.0, 1.0);
    CapEngine::Vector v2(0.25, 0.25, 0.25);
    CapEngine::Vector v3 = v1 + v2;
    CPPUNIT_ASSERT(v3.x == 1.25 && v3.y == 1.25 && v3.z == 1.25);
  }

  void testOperatorEquals(){
    CapEngine::Vector v1(1.0, 1.0, 1.0);
    CapEngine::Vector v2(2.0, 2.0, 2.0);
    v1 = v2;
    CPPUNIT_ASSERT(v1.x == 2.0 && v1.y == 2.0 && v1.z == 2.0);
  }

  CPPUNIT_TEST_SUITE(VectorTest);
  CPPUNIT_TEST(testVectorAdditionLessThanOne);
  CPPUNIT_TEST(testOperatorEquals);
  CPPUNIT_TEST_SUITE_END();

};

#endif //VECTORTEST
