#ifndef CAPENGINE_CAMERA2DTEST_H
#define CAPENGINE_CAMERA2DTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../VideoManager.h"
#include "../camera2d.h"
#include "../locator.h"

namespace CapEngine
{

class Camera2dTest : public CppUnit::TestFixture
{
   public:
    void testGetViewingRectangle();
    void testCenter();
    void testToScreenCoords();

    CPPUNIT_TEST_SUITE(Camera2dTest);
    CPPUNIT_TEST(testGetViewingRectangle);
    CPPUNIT_TEST(testCenter);
    CPPUNIT_TEST(testToScreenCoords);
    CPPUNIT_TEST_SUITE_END();

   private:
    VideoManager *m_pPrevVideoManager;
    std::unique_ptr<VideoManager> m_pMockVideoManager;
};

//! Tests getthing the viewing rectangle
void Camera2dTest::testGetViewingRectangle()
{
    Camera2d camera(5, 5);
    Rectangle expected(0, 0, 5, 5);
    const Rectangle &actual = camera.getViewingRectangle();
    CPPUNIT_ASSERT(expected == actual);
}

//! tests centering on an object
void Camera2dTest::testCenter()
{
    Camera2d camera(5, 5);
    Rectangle object(3, 3, 2, 2);
    camera.center(object, std::nullopt);

    Rectangle expected(2, 2, 5, 5);
    const Rectangle &actual = camera.getViewingRectangle();
    CPPUNIT_ASSERT_EQUAL(expected, actual);

    // test with a bounding rectangle
    object.x = 1;
    object.y = 1;
    camera.center(object, Rectangle(0, 0, 100, 100));
    expected = Rectangle(0, 0, 5, 5);
    CPPUNIT_ASSERT_EQUAL(expected, actual);
}

//! tests converting a rect to screen coordinates via the camera
void Camera2dTest::testToScreenCoords()
{
    Camera2d camera(5, 5);
    Rectangle window(0, 0, 5, 5);

    Rectangle rect(2, 0, 2, 2);

    // y flip only
    Rectangle expected(2, 2, 2, 2);
    Rectangle actual =
        toScreenCoords(camera, rect, window.width, window.height, true);
    CPPUNIT_ASSERT_EQUAL(expected, actual);

    // no y flip
    expected = Rectangle(2, 0, 2, 2);
    actual = toScreenCoords(camera, rect, window.width, window.height, false);
    CPPUNIT_ASSERT_EQUAL(expected, actual);

    // translated with y flip
    // todo: Fix
    // Rectangle object(0, 3, 2, 2);
    // camera.center(object, std::nullopt);

    // actual = toScreenCoords(camera, object, window.width, window.height,
    // true); expected = Rectangle(1, 7, 2, 2); CPPUNIT_ASSERT_EQUAL(expected,
    // actual);
}

} // namespace CapEngine

#endif // CAPENGINE_CAMERA2DTEST_H
