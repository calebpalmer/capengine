#include <gtest/gtest.h>

#include "../collision.h"
#include "../locator.h"
#include "../scanconvert.h"

namespace CapEngine::testing {

TEST(BitmapCollisionTest, TestBitmapCollisions)
{
    auto& videoManager = Locator::getVideoManager();
    SurfacePtr surface = videoManager.createSurfacePtr(20, 20);
    fillRectangle(surface.get(), Rect{0, 0, 20, 20}, Colour{255, 255, 255}, CoordinateSystem::YUP);
    fillRectangle(surface.get(), Rect{5, 0, 5, 10}, Colour{0, 0, 0}, CoordinateSystem::YUP);

    // no collisions
    {
        Rectangle rect{0.0, 0.0, 2.0, 2.0};
        std::vector<std::pair<CollisionType, Vector>> collisions = detectBitmapCollision(rect, surface.get());
        ASSERT_EQ(0, collisions.size());
    }

    // collision on bottom (rect straddles the top edge of the black strip)
    {
        Rectangle rect{5.0, 8.0, 5.0, 5.0};
        std::vector<std::pair<CollisionType, Vector>> collisions = detectBitmapCollision(rect, surface.get());
        ASSERT_EQ(3, collisions.size());
    }

    // rect to the right of the black strip - no collisions
    {
        Rectangle rect{12.0, 0.0, 5.0, 5.0};
        std::vector<std::pair<CollisionType, Vector>> collisions = detectBitmapCollision(rect, surface.get());
        ASSERT_EQ(0, collisions.size());
    }

    // rect above the black strip - no collisions
    {
        Rectangle rect{5.0, 12.0, 5.0, 4.0};
        std::vector<std::pair<CollisionType, Vector>> collisions = detectBitmapCollision(rect, surface.get());
        ASSERT_EQ(0, collisions.size());
    }

    // rect fully inside black strip - all four sides collide
    {
        Rectangle rect{5.0, 1.0, 4.0, 4.0};
        std::vector<std::pair<CollisionType, Vector>> collisions = detectBitmapCollision(rect, surface.get());
        ASSERT_EQ(4, collisions.size());
    }
}

}  // namespace CapEngine::testing
