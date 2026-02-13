#include <capengine/game_management.h>
#include <gtest/gtest.h>

#include "camera2d_test.h"
#include "collision_test.h"
#include "test_colour.h"
#include "test_tiledmap.h"
#include "test_tiledobjectgroup.h"
#include "test_tiledtilelayer.h"
#include "test_tiledtileset.h"
#include "testenvironment.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    auto env = *AddGlobalTestEnvironment(CapEngine::testing::TestEnvironment::instance());
    return RUN_ALL_TESTS();
}
