#include <gtest/gtest.h>

#include <optional>

#include "../collision.h"

namespace CapEngine::testing {

TEST(CollisionTest, TestDetectBoxCollision_IntersectionRectangle)
{
    // no collision
    {
        Rectangle a{0, 0, 2, 2};
        Rectangle b{3, 3, 2, 2};

        ASSERT_EQ(std::nullopt, detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle));
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{3, 0, 4, 4};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_RIGHT, collision->collisionType);
        ASSERT_EQ((Vector{1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{3.5, 2.0}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{2, 3, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_TOP, collision->collisionType);
        ASSERT_EQ((Vector{0.0, 1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{3.0, 3.5}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{3, 3, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_RIGHT, collision->collisionType);
        ASSERT_EQ((Vector{1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{3.5, 3.5}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{4, 4, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_EQ(std::nullopt, collision);
    }

    {
        Rectangle a{0, 0, 3, 3};
        Rectangle b{2, 3, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_EQ(std::nullopt, collision);
    }

    {
        Rectangle a{2, 2, 3, 3};
        Rectangle b{0, 0, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_LEFT, collision->collisionType);
        ASSERT_EQ((Vector{-1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{2.5, 2.5}), collision->representativePoint);
    }

    {
        Rectangle a{1, 2, 3, 3};
        Rectangle b{0, 0, 3, 3};

        const std::optional<BoxCollision> collision =
            detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_BOTTOM, collision->collisionType);
        ASSERT_EQ((Vector{0.0, -1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{2.0, 2.5}), collision->representativePoint);
    }
}

TEST(CollisionTest, TestDetectBoxCollision_Simple)
{
    // no collision
    {
        Rectangle a{0, 0, 2, 2};
        Rectangle b{3, 3, 2, 2};

        ASSERT_EQ(std::nullopt, detectBoxCollision(a, b, RepresentativePointMethod::IntersectionRectangle));
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{3, 0, 4, 4};

        const std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_RIGHT, collision->collisionType);
        ASSERT_EQ((Vector{1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{4.0, 2.0}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{2, 3, 3, 3};

        const std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_TOP, collision->collisionType);
        ASSERT_EQ((Vector{0.0, 1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{3.5, 4.0}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{3, 3, 3, 3};

        // prefer left/right collisions
        std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_RIGHT, collision->collisionType);
        ASSERT_EQ((Vector{1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{4.0, 4.0}), collision->representativePoint);

        // prefer up/down collisions
        collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple, false);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_TOP, collision->collisionType);
        ASSERT_EQ((Vector{0.0, 1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{4.0, 4.0}), collision->representativePoint);
    }

    {
        Rectangle a{0, 0, 4, 4};
        Rectangle b{4, 4, 3, 3};

        const std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_EQ(std::nullopt, collision);
    }

    {
        Rectangle a{0, 0, 3, 3};
        Rectangle b{2, 3, 3, 3};

        const std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_EQ(std::nullopt, collision);
    }

    {
        Rectangle a{2, 2, 3, 3};
        Rectangle b{0, 0, 3, 3};

        // prefer left/rigt
        std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_LEFT, collision->collisionType);
        ASSERT_EQ((Vector{-1.0, 0.0}), collision->collisionNormal);
        ASSERT_EQ((Point{2.0, 2.0}), collision->representativePoint);

        collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple, false);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_BOTTOM, collision->collisionType);
        ASSERT_EQ((Vector{0.0, -1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{2.0, 2.0}), collision->representativePoint);
    }

    {
        Rectangle a{1, 2, 3, 3};
        Rectangle b{0, 0, 3, 3};

        const std::optional<BoxCollision> collision = detectBoxCollision(a, b, RepresentativePointMethod::Simple);
        ASSERT_NE(std::nullopt, collision);
        ASSERT_EQ(CollisionType::COLLISION_BOTTOM, collision->collisionType);
        ASSERT_EQ((Vector{0.0, -1.0}), collision->collisionNormal);
        ASSERT_EQ((Point{1.5, 2.0}), collision->representativePoint);
    }
}
}  // namespace CapEngine::testing
