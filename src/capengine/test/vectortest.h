#ifndef VECTORTEST_H
#define VECTORTEST_H

#include "../vector.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace CapEngine
{

class VectorTest : public CppUnit::TestFixture
{
public:
  void testVectorAdditionLessThanOne();
  void testOperatorEquals();
  void testScalarMultiplication();

  CPPUNIT_TEST_SUITE(VectorTest);
  CPPUNIT_TEST(testVectorAdditionLessThanOne);
  CPPUNIT_TEST(testOperatorEquals);
  CPPUNIT_TEST(testScalarMultiplication);
  CPPUNIT_TEST_SUITE_END();
};

void VectorTest::testVectorAdditionLessThanOne()
{
  CapEngine::Vector v1(1.0f, 1.0f, 1.0f);
  CapEngine::Vector v2(0.25f, 0.25f, 0.25f);
  CapEngine::Vector v3 = v1 + v2;
  CPPUNIT_ASSERT(v3.x == 1.25f && v3.y == 1.25f && v3.z == 1.25f);
}

void VectorTest::testOperatorEquals()
{
  CapEngine::Vector v1(1.0f, 1.0f, 1.0f);
  CapEngine::Vector v2(2.0f, 2.0f, 2.0f);
  v1 = v2;
  CPPUNIT_ASSERT(v1.x == 2.0f && v1.y == 2.0f && v1.z == 2.0f);
}

void VectorTest::testScalarMultiplication()
{
  real scalar = 1.2;
  Vector v(1.0f, 2.0f, 3.0f);

  Vector result = v * scalar;
  Vector expected(1.2f, 2.4f, 3.6f);
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

} // namespace CapEngine

#endif // VECTORTEST
