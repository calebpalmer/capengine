#ifndef RECTANGLETEST_H
#define RECTANGLETEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../collision.h"

class RectangleTest : public CppUnit::TestFixture {
  public:
    CapEngine::Rectangle testRect;
    double delta = 0.0001;

    void setUp() {
      testRect = CapEngine::Rectangle(1.0, 1.0, 5.0, 5.0); 
    }

    void tearDown() {}
    
    void testCreateRectangle(){
      CapEngine::Rectangle rect(1.0, 2.0, 5.0, 3.2);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, rect.x, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, rect.y, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, rect.width, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2, rect.height, delta);
    }
    
    void testRaiseBottom(){
      auto newRect = testRect.raiseBottom(2);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.x, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.y, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.width, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, newRect.height, delta);

    }

    void testLowerTop(){
      auto newRect = testRect.lowerTop(2);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.x, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, newRect.y, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.width, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.height, delta);

    }

    void testNarrowRight(){
      auto newRect = testRect.narrowRight(2);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.x, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.y, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, newRect.width, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.height, delta);

    }

    void testNarrowLeft(){
      auto newRect = testRect.narrowLeft(2);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, newRect.x, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, newRect.y, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.width, delta);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, newRect.height, delta);

    }

    CPPUNIT_TEST_SUITE(RectangleTest);
    CPPUNIT_TEST(testCreateRectangle);
    CPPUNIT_TEST(testRaiseBottom);
    CPPUNIT_TEST(testLowerTop);
    CPPUNIT_TEST(testNarrowRight);
    CPPUNIT_TEST(testNarrowLeft);
    CPPUNIT_TEST_SUITE_END();

};

#endif // RECTANGLETEST_H
