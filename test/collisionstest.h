#ifndef CAPENGINE_COLLISIONTEST_H
#define CAPENGINE_COLLISIONTEST_H

#include <algorithm>
#include <cppunit/TestAssert.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../VideoManager.h"
#include "../captypes.h"
#include "../collision.h"
#include "../game_management.h"
#include "../locator.h"
#include "../scanconvert.h"
#include "SDL_rect.h"

#include <memory>

namespace CapEngine
{
namespace Testing
{

class CollisionsTests : public CppUnit::TestFixture
{
public:
  void setUp();
  void tearDown();
  void testBitmapCollision_none();
  void testBitmapCollision_top();
  void testBitmapCollision_bottom();

  CPPUNIT_TEST_SUITE(CollisionsTests);
  CPPUNIT_TEST(testBitmapCollision_none);
  CPPUNIT_TEST(testBitmapCollision_top);
  CPPUNIT_TEST(testBitmapCollision_bottom);
  CPPUNIT_TEST_SUITE_END();
};

void CollisionsTests::setUp()
{
  WindowParams windowParams = {"test", 320,   210,   32,    false,
                               false,  false, false, "test"};
  init(windowParams);
  Locator::videoManager;
}

void CollisionsTests::tearDown() { Locator::videoManager->shutdown(); }

void CollisionsTests::testBitmapCollision_none()
{
  SurfacePtr surface = Locator::videoManager->createSurfacePtr(20, 20);
  fillRectangle(surface.get(), SDL_Rect{0, 0, 20, 20},
                Colour{255, 255, 255, 255});
  fillRectangle(surface.get(), SDL_Rect{0, 0, 2, 2}, Colour{0, 0, 0, 255});

  const std::vector<PixelCollision> collisions =
      detectBitmapCollisions({2, 2, 2, 2}, surface.get());

  CPPUNIT_ASSERT_EQUAL(static_cast<decltype(collisions)::size_type>(0),
                       collisions.size());
}

void CollisionsTests::testBitmapCollision_top()
{
  SurfacePtr surface = Locator::videoManager->createSurfacePtr(20, 20);
  fillRectangle(surface.get(), SDL_Rect{0, 0, 20, 20},
                Colour{255, 255, 255, 255});
  fillRectangle(surface.get(), SDL_Rect{0, 0, 2, 2}, Colour{0, 0, 0, 255});

  const std::vector<PixelCollision> collisions =
      detectBitmapCollisions({1, 1, 2, 2}, surface.get());

  CPPUNIT_ASSERT_EQUAL(static_cast<decltype(collisions)::size_type>(2),
                       collisions.size());
}

void CollisionsTests::testBitmapCollision_bottom()
{
  SurfacePtr surface = Locator::videoManager->createSurfacePtr(20, 20);
  fillRectangle(surface.get(), SDL_Rect{0, 0, 20, 20},
                Colour{255, 255, 255, 255});
  fillRectangle(surface.get(), SDL_Rect{0, 10, 2, 2}, Colour{0, 0, 0, 255});

  std::vector<PixelCollision> collisions =
      detectBitmapCollisions({0, 8, 2, 2}, surface.get());

  CPPUNIT_ASSERT_EQUAL(static_cast<decltype(collisions)::size_type>(0),
                       collisions.size());

  collisions = detectBitmapCollisions(SDL_Rect{0, 8, 2, 5}, surface.get());

  for (auto &&collision : collisions) {
    std::cout << collision << std::endl;
  }
  CPPUNIT_ASSERT_EQUAL(static_cast<decltype(collisions)::size_type>(2),
                       collisions.size());
}

} // namespace Testing
} // namespace CapEngine

#endif // CAPENGINE_COLLSIONTEST_H
