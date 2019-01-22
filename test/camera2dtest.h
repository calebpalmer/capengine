#ifndef CAPENGINE_CAMERA2DTEST_H
#define CAPENGINE_CAMERA2DTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../camera2d.h"
#include "../VideoManager.h"

namespace CapEngine {

namespace {

class MockVideoManager : public VideoManager {
 public:
  MockVideoManager() {}
  bool isValidWindowId(uint32_t windowId) const override {
    return true;
  }

  int getWindowHeight(uint32_t) override{
    return 10;
  }
};

} // namespace

class Camera2dTest : public CppUnit::TestFixture {
 public:
  void setUp() override;
  void tearDown() override;

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

void Camera2dTest::setUp(){
	m_pPrevVideoManager = Locator::videoManager;
	m_pMockVideoManager.reset(new MockVideoManager);
	Locator::videoManager = m_pMockVideoManager.get();
}

void Camera2dTest::tearDown(){
	Locator::videoManager = m_pPrevVideoManager;
	m_pMockVideoManager->shutdown();
}

//! Tests getthing the viewing rectangle
void Camera2dTest::testGetViewingRectangle(){
	Camera2d camera(5, 5);
	Rectangle expected(0, 0, 5, 5);
	const Rectangle& actual = camera.getViewingRectangle();
	CPPUNIT_ASSERT(expected == actual);
}

//! tests centering on an object
void Camera2dTest::testCenter(){
	Camera2d camera(5, 5);
	Rectangle object(3, 3, 2, 2);
	camera.center(object, std::nullopt);

	Rectangle expected(2, 2, 5, 5);
	const Rectangle& actual = camera.getViewingRectangle();
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	// test with a bounding rectangle
	object.x = 1;
	object.y = 1;
	camera.center(object, Rectangle(0, 0, 100, 100));
	expected = Rectangle(0, 0, 5, 5);
	CPPUNIT_ASSERT_EQUAL(expected, actual);
	 
}

//! tests converting a rect to screen coordinates via the camera
void Camera2dTest::testToScreenCoords(){
	Camera2d camera(5, 5);

	Rectangle rect(2, 0, 2, 2);
	int windowHeight = Locator::videoManager->getWindowHeight(0);

	// y flip only
	Rectangle expected(2, 8, 2, 2);
	Rectangle actual = toScreenCoords(camera, rect, 0, true);
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	// no y flip
	expected = Rectangle(2, 0, 2, 2);
	actual = toScreenCoords(camera, rect, 0, false);
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	// translated with y flip
	Rectangle object(0, 3, 2, 2);
	camera.center(object, std::nullopt);

	actual = toScreenCoords(camera, object, 0, true);
	expected = Rectangle(1, 7, 2, 2);
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

}

#endif // CAPENGINE_CAMERA2DTEST_H
