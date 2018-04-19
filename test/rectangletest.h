#ifndef RECTANGLETEST_H
#define RECTANGLETEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../collision.h"

class RectangleTest : public CppUnit::TestFixture {
  public:
    CapEngine::Rectangle testRect;

    void setUp() {
      testRect = CapEngine::Rectangle(1, 1, 5, 5); 
    }

    void tearDown() {}
    
    void testCreateRectangle(){
      CapEngine::Rectangle rect(1, 2, 5, 3);
      CPPUNIT_ASSERT_EQUAL(1, rect.x);
      CPPUNIT_ASSERT_EQUAL(2, rect.y);
      CPPUNIT_ASSERT_EQUAL(5, rect.width);
      CPPUNIT_ASSERT_EQUAL(3, rect.height);
    }
    
    void testRaiseBottom(){
      auto newRect = testRect.raiseBottom(2);

      CPPUNIT_ASSERT_EQUAL(1, newRect.x);
      CPPUNIT_ASSERT_EQUAL(1, newRect.y);
      CPPUNIT_ASSERT_EQUAL(5, newRect.width);
      CPPUNIT_ASSERT_EQUAL(3, newRect.height);

    }

    void testLowerTop(){
      auto newRect = testRect.lowerTop(2);

      CPPUNIT_ASSERT_EQUAL(1, newRect.x);
      CPPUNIT_ASSERT_EQUAL(3, newRect.y);
      CPPUNIT_ASSERT_EQUAL(5, newRect.width);
      CPPUNIT_ASSERT_EQUAL(5, newRect.height);

    }

    void testNarrowRight(){
      auto newRect = testRect.narrowRight(2);

      CPPUNIT_ASSERT_EQUAL(1, newRect.x);
      CPPUNIT_ASSERT_EQUAL(1, newRect.y);
      CPPUNIT_ASSERT_EQUAL(3, newRect.width);
      CPPUNIT_ASSERT_EQUAL(5, newRect.height);

    }

    void testNarrowLeft(){
      auto newRect = testRect.narrowLeft(2);

      CPPUNIT_ASSERT_EQUAL(3, newRect.x);
      CPPUNIT_ASSERT_EQUAL(1, newRect.y);
      CPPUNIT_ASSERT_EQUAL(5, newRect.width);
      CPPUNIT_ASSERT_EQUAL(5, newRect.height);

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
