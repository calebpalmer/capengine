#ifndef RECTANGLETEST_H
#define RECTANGLETEST_H

#include "../collision.h"
#include "../utils.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace CapEngine
{

class RectangleTest : public CppUnit::TestFixture
{
public:
  void setUp();
  void testCreateRectangle();
  void testRaiseBottom();
  void testLowerTop();
  void testNarrowRight();
  void testNarrowLeft();
  void testIntersect();

  CPPUNIT_TEST_SUITE(RectangleTest);
  CPPUNIT_TEST(testCreateRectangle);
  CPPUNIT_TEST(testRaiseBottom);
  CPPUNIT_TEST(testLowerTop);
  CPPUNIT_TEST(testNarrowRight);
  CPPUNIT_TEST(testNarrowLeft);
  CPPUNIT_TEST(testIntersect);
  CPPUNIT_TEST_SUITE_END();

private:
  CapEngine::Rectangle testRect;
};

void RectangleTest::setUp() { testRect = CapEngine::Rectangle(1, 1, 5, 5); }

void RectangleTest::testCreateRectangle()
{
  CapEngine::Rectangle rect(1, 2, 5, 3);
  CPPUNIT_ASSERT_EQUAL(1, rect.x);
  CPPUNIT_ASSERT_EQUAL(2, rect.y);
  CPPUNIT_ASSERT_EQUAL(5, rect.width);
  CPPUNIT_ASSERT_EQUAL(3, rect.height);
}

void RectangleTest::testRaiseBottom()
{
  auto newRect = testRect.raiseBottom(2);

  CPPUNIT_ASSERT_EQUAL(1, newRect.x);
  CPPUNIT_ASSERT_EQUAL(1, newRect.y);
  CPPUNIT_ASSERT_EQUAL(5, newRect.width);
  CPPUNIT_ASSERT_EQUAL(3, newRect.height);
}

void RectangleTest::testLowerTop()
{
  auto newRect = testRect.lowerTop(2);

  CPPUNIT_ASSERT_EQUAL(1, newRect.x);
  CPPUNIT_ASSERT_EQUAL(3, newRect.y);
  CPPUNIT_ASSERT_EQUAL(5, newRect.width);
  CPPUNIT_ASSERT_EQUAL(5, newRect.height);
}

void RectangleTest::testNarrowRight()
{
  auto newRect = testRect.narrowRight(2);

  CPPUNIT_ASSERT_EQUAL(1, newRect.x);
  CPPUNIT_ASSERT_EQUAL(1, newRect.y);
  CPPUNIT_ASSERT_EQUAL(3, newRect.width);
  CPPUNIT_ASSERT_EQUAL(5, newRect.height);
}

void RectangleTest::testNarrowLeft()
{
  auto newRect = testRect.narrowLeft(2);

  CPPUNIT_ASSERT_EQUAL(3, newRect.x);
  CPPUNIT_ASSERT_EQUAL(1, newRect.y);
  CPPUNIT_ASSERT_EQUAL(5, newRect.width);
  CPPUNIT_ASSERT_EQUAL(5, newRect.height);
}

// Tests intersecting two rectangles
void RectangleTest::testIntersect()
{
  SDL_Rect r1 = {0, 0, 4, 4};
  SDL_Rect r2 = {1, 1, 5, 5};

  SDL_Rect expected = {1, 1, 4, 4};
  std::optional<SDL_Rect> actual = intersectRects(r1, r2);

  CPPUNIT_ASSERT(actual != std::nullopt);
  CPPUNIT_ASSERT(expected.x == actual->x && expected.y == actual->y &&
                 expected.w == actual->w && expected.h == actual->h);
}

} // namespace CapEngine

#endif // RECTANGLETEST_H
