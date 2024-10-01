#include "../camera2d.h"
#include "../captypes.h"
#include "../vector.h"
#include "testenvironment.h"

#include <gtest/gtest.h>

namespace CapEngine::testing {
	TEST(Camera2DTest, TestWindowToLocalCoordinatese){
		Camera2d camera{10, 10};
		camera.setPosition(2, 2);

		Rectangle rect{0, 0, 20, 20};
		Vector clickedLocation{6, 6};

		Vector localCoords = windowToLocalCoordinate(camera, clickedLocation, rect, TestEnvironment::instance()->getWindowId(), false);
		Vector expected{8, 8};
		ASSERT_EQ(expected, localCoords);
	}
} // namespace CapEngine::testing
